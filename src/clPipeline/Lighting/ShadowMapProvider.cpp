#include <glm/gtx/transform.hpp>

#include <mtt/clPipeline/Lighting/ShadowMapProvider.h>
#include <mtt/render/DrawPlan/DrawPlan.h>
#include <mtt/render/SceneGraph/DrawVisitor.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/clip.h>

using namespace mtt;
using namespace mtt::clPipeline;

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

void ShadowMapProvider::setTargetField(AbstractField* newField) noexcept
{
  _targetField = newField;
}

ImageView& ShadowMapProvider::getShadowMap( const CameraNode& camera,
                                            DrawPlanBuildInfo& buildInfo)
{
  DrawPlan& drawPlan = buildInfo.drawPlan;

  std::vector<AbstractFramePlan*> existingPlans =
                                        drawPlan.findFramePlans(_frameBuilder);
  ImageView* existingMap = _tryUseExistingPlan( existingPlans,
                                                camera,
                                                drawPlan,
                                                *buildInfo.currentFramePlan);
  if(existingMap != nullptr) return *existingMap;

  size_t frameIndex = existingPlans.size();
  FrameRecord& frameRecord = _getOrCreateFrame(frameIndex);
  OneTargetFrameBuilder::Frame& frame = *frameRecord.frame;

  _buildNewMap( camera,
                frame,
                drawPlan,
                *buildInfo.currentFramePlan,
                buildInfo.rootViewInfo);

  return *frameRecord.samplerImageView;
}

ImageView* ShadowMapProvider::_tryUseExistingPlan(
                                  const std::vector<AbstractFramePlan*>& plans,
                                  const CameraNode& renderCamera,
                                  DrawPlan& drawPlan,
                                  const AbstractFramePlan& dependentFrame)
{
  glm::mat4 viewProjectionMatrix =
                    renderCamera.projectionMatrix() * renderCamera.viewMatrix();

  for (AbstractFramePlan* framePlan : plans)
  {
    ShadowmapBuilder::ShadowMapFramePlan& shadowmapPlan =
                static_cast<ShadowmapBuilder::ShadowMapFramePlan&>(*framePlan);
    if (shadowmapPlan.viewProjectionMatrix == viewProjectionMatrix)
    {
      for (FrameRecord& frameRecord : _framePools[_currentPoolIndex])
      {
        if (frameRecord.frame.get() == &shadowmapPlan.frame())
        {
          drawPlan.addDependency(*framePlan, dependentFrame);
          return frameRecord.samplerImageView.get();
        }
      }
    }
  }
  return nullptr;
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

void ShadowMapProvider::_buildNewMap( const CameraNode& renderCamera,
                                      OneTargetFrameBuilder::Frame& frame,
                                      DrawPlan& drawPlan,
                                      const AbstractFramePlan& dependentFrame,
                                      ViewInfo& rootViewInfo)
{
  VkViewport viewport { 0.f,
                        0.f,
                        float(_frameExtent.x),
                        float(_frameExtent.y),
                        0.f,
                        1.f};
  VkRect2D scissor {0, 0, uint32_t(_frameExtent.x), uint32_t(_frameExtent.y)};

  std::unique_ptr<AbstractFramePlan> framePlanPtr =
                                          _frameBuilder.createFramePlan(frame);
  ShadowmapBuilder::ShadowMapFramePlan& framePlan = 
              static_cast<ShadowmapBuilder::ShadowMapFramePlan&>(*framePlanPtr);
  framePlan.viewProjectionMatrix =
                    renderCamera.projectionMatrix() * renderCamera.viewMatrix();
  drawPlan.addFramePlan(std::move(framePlanPtr));
  drawPlan.addDependency(framePlan, dependentFrame);

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
}
