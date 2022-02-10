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

ShadowMapProvider::Area ShadowMapProvider::_getTopArea(
                                    DrawPlanBuildInfo& buildInfo) const noexcept
{
  glm::mat4& clipToView = buildInfo.drawMatrices.clipToViewMatrix;
  glm::mat4 toLightView =
                        glm::inverse(buildInfo.drawMatrices.localToViewMatrix);
  glm::mat4 toLightClip = _camera.projectionMatrix();
  glm::mat4 pointsTransform = toLightClip * toLightView * clipToView;

  glm::vec4 frustumPoints[] = { pointsTransform * glm::vec4(-1, -1, 1, 1),
                                pointsTransform * glm::vec4( 1, -1, 1, 1),
                                pointsTransform * glm::vec4( 1,  1, 1, 1),
                                pointsTransform * glm::vec4(-1,  1, 1, 1),
                                pointsTransform * glm::vec4(-1, -1, 0, 1),
                                pointsTransform * glm::vec4( 1, -1, 0, 1),
                                pointsTransform * glm::vec4( 1,  1, 0, 1),
                                pointsTransform * glm::vec4(-1,  1, 0, 1)};
  glm::vec4 clipArea{1,1,-1,-1};

  for(glm::vec4& point : frustumPoints)
  {
    if(point.x > point.w) point.x = point.w;
    if(point.x < -point.w) point.x = -point.w;
    point.x /= point.w;

    if(point.y > point.w) point.y = point.w;
    if(point.y < -point.w) point.y = -point.w;
    point.y /= point.w;

    clipArea.x = std::min(clipArea.x, point.x);
    clipArea.y = std::min(clipArea.y, point.y);
    clipArea.z = std::max(clipArea.z, point.x);
    clipArea.w = std::max(clipArea.w, point.y);
  }

  ShadowMapProvider::Area result;
  result.topleftCorner = glm::vec2(clipArea.x, clipArea.y);
  result.size = glm::vec2(clipArea.z - clipArea.x, clipArea.w - clipArea.y);
  return result;
}

glm::vec2 ShadowMapProvider::_getCascadeDirectionPoint(
                                          DrawPlanBuildInfo& buildInfo,
                                          glm::vec2 startPoint) const noexcept
{
  glm::mat4 toLightView =
                        glm::inverse(buildInfo.drawMatrices.localToViewMatrix);
  glm::mat4 toLightClip = _camera.projectionMatrix();
  glm::mat4 endPointTransform = toLightClip * toLightView;
  glm::vec4 endPoint = endPointTransform *
                        glm::vec4(buildInfo.currentViewInfo.viewPosition, 1.f);
  glm::vec4 startPoint4d(startPoint, .5f, 1);
  clipSegment(startPoint4d, endPoint);
  glm::vec2 directionPoint = endPoint / endPoint.w;

  return directionPoint;
}

ShadowMapProvider::Area ShadowMapProvider::_alignArea(
                          const ShadowMapProvider::Area& source) const noexcept
{
  float areaSize = std::max(source.size.x, source.size.y);
  float sizeOrder = ceil(log2(areaSize));
  areaSize = exp2(sizeOrder);
  if(areaSize > 2.f) areaSize = 2.f;

  float granularity = areaSize / _frameExtent.x * 2.f;

  glm::vec2 sourceCenter = source.topleftCorner + source.size / 2.f;
  glm::vec2 alignedCorner = sourceCenter - glm::vec2(areaSize) / 2.f;
  alignedCorner = glm::round(alignedCorner / granularity) * granularity;
  alignedCorner = glm::clamp(alignedCorner, -1.f, 1.f);

  ShadowMapProvider::Area alignedArea;
  alignedArea.topleftCorner = alignedCorner;
  alignedArea.size = glm::vec2(areaSize);

  return alignedArea;
}

ShadowMapProvider::CascadeInfo ShadowMapProvider::createShadowMap(
                                                  size_t cascadeSize,
                                                  DrawPlanBuildInfo& buildInfo)
{
  if(cascadeSize == 0) return CascadeInfo{};

  ShadowMapProvider::Area area = _getTopArea(buildInfo);
  if(area.size.x <= 0 || area.size.y <= 0) return CascadeInfo();

  glm::vec2 centralPoint = area.topleftCorner + area.size / 2.f;
  glm::vec2 directionPoint = _getCascadeDirectionPoint(buildInfo, centralPoint);
  glm::vec2 shift = directionPoint - centralPoint;

  CascadeInfo result;
  result.reserve(cascadeSize);

  for(size_t layerIndex = 0; layerIndex < cascadeSize; layerIndex++)
  {
    ShadowMapProvider::Area alignedArea = _alignArea(area);

    ShadowMapInfo info{};
    info.map = &_getShadowMap(alignedArea,
                              buildInfo.drawPlan,
                              *buildInfo.currentFramePlan,
                              buildInfo.rootViewInfo);

    glm::vec2 uvAreaSize = alignedArea.size / 2.f;
    glm::vec2 uvAreaCorner =
              alignedArea.topleftCorner * glm::vec2(.5f, .5f) + glm::vec2(.5f);

    info.coordCorrection.multiplicator = 1.f / uvAreaSize.x;
    info.coordCorrection.shift = glm::vec2( -uvAreaCorner.x / uvAreaSize.x,
                                            -uvAreaCorner.y / uvAreaSize.y);
    result.push_back(info);

    shift /= 2.f;

    glm::vec2 center = area.topleftCorner + area.size / 2.f;
    center = center + shift;
    area.size /= 2.f;
    area.topleftCorner = center - area.size / 2.f;
  }

  return result;
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

ImageView& ShadowMapProvider::_getShadowMap(
                                        const Area& mapPart,
                                        DrawPlan& drawPlan,
                                        const AbstractFramePlan& dependentFrame,
                                        ViewInfo& rootViewInfo)
{
  CameraNode renderCamera;
  _setupRenderCamera(renderCamera, mapPart);

  std::vector<AbstractFramePlan*> existingPlans =
                                              drawPlan.findPlans(_frameBuilder);
  ImageView* existingMap = _tryUseExistingPlan( existingPlans,
                                                renderCamera,
                                                drawPlan,
                                                dependentFrame);
  if(existingMap != nullptr) return *existingMap;

  size_t frameIndex = existingPlans.size();
  FrameRecord& frameRecord = _getOrCreateFrame(frameIndex);
  OneTargetFrameBuilder::Frame& frame = *frameRecord.frame;

  _buildNewMap(renderCamera, frame, drawPlan, dependentFrame, rootViewInfo);

  return *frameRecord.samplerImageView;
}

ImageView* ShadowMapProvider::_tryUseExistingPlan(
                                  const std::vector<AbstractFramePlan*>& plans,
                                  CameraNode& renderCamera,
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

void ShadowMapProvider::_buildNewMap( CameraNode& renderCamera,
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
