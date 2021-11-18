#include <glm/gtx/transform.hpp>

#include <mtt/DrawImpl/Lighting/ShadowMapProvider.h>
#include <mtt/Render/DrawPlan/DrawPlan.h>
#include <mtt/Render/SceneGraph/DrawVisitor.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

static constexpr VkImageUsageFlags shadowmapUsage = VK_IMAGE_USAGE_SAMPLED_BIT;

ShadowMapProvider::ShadowMapProvider( size_t framePoolsNumber,
                                      glm::uvec2 frameExtent,
                                      LogicalDevice& device) :
  _frameExtent(frameExtent),
  _frameBuilder(shadowmapFormat, shadowmapLayout, shadowmapUsage, device),
  _framePools(framePoolsNumber),
  _currentPoolIndex(0),
  _targetField(nullptr)
{
  if(framePoolsNumber == 0) Abort("ShadowMapProvider::ShadowMapProvider: framePoolsNumber = 0");
  if(frameExtent.x == 0 || frameExtent.y == 0) Abort("ShadowMapProvider::ShadowMapProvider: wrong frame extent.");
}

void ShadowMapProvider::setFrameExtent(glm::uvec2 frameExtent)
{
  if (frameExtent.x == 0 || frameExtent.y == 0) Abort("ShadowMapProvider::setFrameExtent: wrong frame extent.");

  for(FramePool& pool : _framePools) pool.clear();
  _frameExtent = frameExtent;
}

void ShadowMapProvider::setTargetField(VisitedField* newField) noexcept
{
  _targetField = newField;
}

ImageView& ShadowMapProvider::createShadowMap(
                                        const Area& mapPart,
                                        float blurLevel,
                                        DrawPlan& drawPlan,
                                        const AbstractFramePlan& dependentFrame,
                                        ViewInfo& rootViewInfo)
{
  size_t frameIndex = drawPlan.findPlans(_frameBuilder).size();
  FrameRecord& frameRecord = _getOrCreateFrame(frameIndex);
  OneTargetFrameBuilder::Frame& frame = *frameRecord.frame;

  VkViewport viewport { 0.f,
                        0.f,
                        float(_frameExtent.x),
                        float(_frameExtent.y),
                        0.f,
                        1.f};
  VkRect2D scissor {0, 0, uint32_t(_frameExtent.x), uint32_t(_frameExtent.y)};

  std::unique_ptr<AbstractFramePlan> framePlanPtr =
                                      _frameBuilder.createFramePlan(frame,
                                                                    blurLevel);
  AbstractFramePlan& framePlan = *framePlanPtr;
  drawPlan.addFramePlan(std::move(framePlanPtr));
  drawPlan.addDependency(framePlan, dependentFrame);

  CameraNode renderCamera;
  _setupRenderCamera(renderCamera, mapPart);

  DrawPlanBuildInfo planBuildInfo(drawPlan);
  planBuildInfo.adjustFrameRender(viewport,
                                  scissor,
                                  framePlan,
                                  renderCamera,
                                  &rootViewInfo);

  if(_targetField != nullptr)
  {
    DrawVisitor drawVisitor(planBuildInfo);
    ViewFrustum localFrustum = planBuildInfo.viewFrustum;
    localFrustum.fastTranslate(
                  glm::transpose(planBuildInfo.drawMatrices.localToViewMatrix));
    _targetField->pass(drawVisitor, localFrustum);
  }

  return *frameRecord.samplerImageView;
}

ShadowMapProvider::FrameRecord&
                              ShadowMapProvider::_getOrCreateFrame(size_t index)
{
  if(index == 0)
  {
    _currentPoolIndex = (_currentPoolIndex + 1) % _framePools.size();
  }

  FramePool& pool = _framePools[_currentPoolIndex];
  while(pool.size() < index + 1)
  {
    FrameRecord newRecord;
    newRecord.frame = _frameBuilder.createFrame(_frameExtent);

    Image& image = newRecord.frame->targetImage();
    VkComponentMapping colorMapping;
    colorMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    colorMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    colorMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    colorMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    VkImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = image.mipmapCount();
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = image.arraySize();

    Ref<ImageView> imageView( new ImageView(image,
                                            VK_IMAGE_VIEW_TYPE_2D,
                                            colorMapping,
                                            subresourceRange));
    newRecord.samplerImageView = std::move(imageView);

    pool.push_back(std::move(newRecord));
  }

  return pool[index];
}

void ShadowMapProvider::_setupRenderCamera( CameraNode& renderCamera,
                                            const Area& mapPart) const noexcept
{
  renderCamera.setTransformMatrix(_camera.transformMatrix());

  glm::mat4 projectionMatrix = _camera.projectionMatrix();

  glm::vec2 areaCenter = mapPart.topleftCorner + mapPart.size / 2.f;
  glm::vec3 projectionShift(-areaCenter, 0.f);
  projectionMatrix = glm::translate(projectionShift) * projectionMatrix;

  glm::vec3 projectionScale(2.f / mapPart.size.x,
                            2.f / mapPart.size.y,
                            1.f);
  projectionMatrix = glm::scale(projectionScale) * projectionMatrix;

  renderCamera.setProjectionMatrix(projectionMatrix);
}
