#include <glm/gtx/transform.hpp>

#include <mtt/clPipeline/Lighting/ShadowMapProvider.h>
#include <mtt/render/DrawPlan/DrawPlan.h>
#include <mtt/render/SceneGraph/DrawVisitor.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/clip.h>

using namespace mtt;
using namespace mtt::clPipeline;

static constexpr VkImageUsageFlags shadowmapUsage =
                                            VK_IMAGE_USAGE_SAMPLED_BIT |
                                            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

ShadowMapProvider::ShadowMapProvider( size_t framePoolsNumber,
                                      glm::uvec2 opaqueMapExtent,
                                      glm::uvec2 transparentMapExtent,
                                      LogicalDevice& device) :
  _opaqueMapExtent(opaqueMapExtent),
  _transparentMapExtent(transparentMapExtent),
  _opaqueFrameBuilder(opaqueMapFormat, shadowmapLayout, device),
  _transparentFrameBuilder(transparentMapFormat, shadowmapLayout, device),
  _framePools(framePoolsNumber),
  _currentPoolIndex(0),
  _targetField(nullptr)
{
  if(framePoolsNumber == 0) Abort("ShadowMapProvider::ShadowMapProvider: framePoolsNumber = 0");
  if(opaqueMapExtent.x == 0 || opaqueMapExtent.y == 0) Abort("ShadowMapProvider::ShadowMapProvider: wrong opaque map extent.");
  if(transparentMapExtent.x == 0 || transparentMapExtent.y == 0) Abort("ShadowMapProvider::ShadowMapProvider: wrong transparent map extent.");
}

void ShadowMapProvider::setMapExtent( glm::uvec2 opaqueMapExtent,
                                      glm::uvec2 transparentMapExtent)
{
  if (opaqueMapExtent.x == 0 || opaqueMapExtent.y == 0) Abort("ShadowMapProvider::setMapExtent: wrong opaque map extent.");
  if (transparentMapExtent.x == 0 || transparentMapExtent.y == 0) Abort("ShadowMapProvider::setMapExtent: wrong transparent map extent.");

  for(FramePool& pool : _framePools) pool.clear();
  _opaqueMapExtent = opaqueMapExtent;
  _transparentMapExtent = transparentMapExtent;
}

void ShadowMapProvider::setTargetField(AbstractField* newField) noexcept
{
  _targetField = newField;
}

ShadowMapProvider::Shadowmaps ShadowMapProvider::getShadowMaps(
                                                  const CameraNode& camera,
                                                  DrawPlanBuildInfo& buildInfo)
{
  DrawPlan& drawPlan = buildInfo.drawPlan;

  std::vector<AbstractFramePlan*> existingPlans =
                                  drawPlan.findFramePlans(_opaqueFrameBuilder);
  FrameRecord* existingMap = _tryUseExistingPlan( existingPlans,
                                                  camera,
                                                  drawPlan,
                                                  *buildInfo.currentFramePlan);
  if(existingMap != nullptr)
  {
    return {existingMap->opaqueMapView.get(),
            existingMap->transparentMapView.get()};
  }

  size_t frameIndex = existingPlans.size();
  FrameRecord& frameRecord = _getOrCreateFrame(frameIndex);

  _buildNewMap( camera,
                *frameRecord.opaqueFrame,
                *frameRecord.transparentFrame,
                drawPlan,
                *buildInfo.currentFramePlan,
                buildInfo.rootViewInfo);

  return {frameRecord.opaqueMapView.get(),
          frameRecord.transparentMapView.get()};
}

ShadowMapProvider::FrameRecord* ShadowMapProvider::_tryUseExistingPlan(
                                  const std::vector<AbstractFramePlan*>& plans,
                                  const CameraNode& renderCamera,
                                  DrawPlan& drawPlan,
                                  const AbstractFramePlan& dependentFrame)
{
  glm::mat4 viewProjectionMatrix =
                    renderCamera.projectionMatrix() * renderCamera.viewMatrix();

  for (AbstractFramePlan* framePlan : plans)
  {
    OpaqueShadowmapBuilder::FramePlan& shadowmapPlan =
                    static_cast<OpaqueShadowmapBuilder::FramePlan&>(*framePlan);
    if (shadowmapPlan.viewProjectionMatrix == viewProjectionMatrix)
    {
      for (FrameRecord& frameRecord : _framePools[_currentPoolIndex])
      {
        if (frameRecord.opaqueFrame.get() == &shadowmapPlan.frame())
        {
          drawPlan.addDependency(*framePlan, dependentFrame);
          return &frameRecord;
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
    mtt::Ref<Image> opaqueMap(new Image(VK_IMAGE_TYPE_2D,
                                        shadowmapLayout,
                                        shadowmapUsage,
                                        0,
                                        opaqueMapFormat,
                                        glm::uvec3(_opaqueMapExtent, 1),
                                        VK_SAMPLE_COUNT_1_BIT,
                                        1,
                                        1,
                                        VK_IMAGE_ASPECT_COLOR_BIT,
                                        _opaqueFrameBuilder.device()));

    mtt::Ref<Image> transparentMap(new Image( VK_IMAGE_TYPE_2D,
                                              shadowmapLayout,
                                              shadowmapUsage,
                                              0,
                                              transparentMapFormat,
                                              glm::uvec3( _transparentMapExtent,
                                                          1),
                                              VK_SAMPLE_COUNT_1_BIT,
                                              1,
                                              1,
                                              VK_IMAGE_ASPECT_COLOR_BIT,
                                              _opaqueFrameBuilder.device()));

    VkComponentMapping colorMapping;
    colorMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    colorMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    colorMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    colorMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    VkImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    Ref<ImageView> opaqueMapView(new ImageView( *opaqueMap,
                                                VK_IMAGE_VIEW_TYPE_2D,
                                                colorMapping,
                                                subresourceRange));

    Ref<ImageView> transparentMapView(new ImageView(*transparentMap,
                                                    VK_IMAGE_VIEW_TYPE_2D,
                                                    colorMapping,
                                                    subresourceRange));

    FrameRecord newRecord;
    newRecord.opaqueFrame = _opaqueFrameBuilder.createFrame(*opaqueMapView);
    newRecord.opaqueMapView = opaqueMapView;
    newRecord.transparentFrame =
                      _transparentFrameBuilder.createFrame(*transparentMapView);
    newRecord.transparentMapView = transparentMapView;
    pool.push_back(std::move(newRecord));
  }

  return pool[index];
}

void ShadowMapProvider::_buildNewMap( const CameraNode& renderCamera,
                                      AbstractFrame& opaqueFrame,
                                      AbstractFrame& transparentFrame,
                                      DrawPlan& drawPlan,
                                      const AbstractFramePlan& dependentFrame,
                                      ViewInfo& rootViewInfo)
{
  if (_targetField == nullptr) return;

  std::unique_ptr<AbstractFramePlan> opaqueFramePlanPtr =
                              _opaqueFrameBuilder.createFramePlan(opaqueFrame);
  OpaqueShadowmapBuilder::FramePlan& opaqueFramePlan =
          static_cast<OpaqueShadowmapBuilder::FramePlan&>(*opaqueFramePlanPtr);
  opaqueFramePlan.viewProjectionMatrix =
                    renderCamera.projectionMatrix() * renderCamera.viewMatrix();
  drawPlan.addFramePlan(std::move(opaqueFramePlanPtr));
  drawPlan.addDependency(opaqueFramePlan, dependentFrame);

  std::unique_ptr<AbstractFramePlan> transparentFramePlanPtr =
                    _transparentFrameBuilder.createFramePlan(transparentFrame);
  AbstractFramePlan& transparentFramePlan = *transparentFramePlanPtr;
  drawPlan.addFramePlan(std::move(transparentFramePlanPtr));
  drawPlan.addDependency(transparentFramePlan, dependentFrame);


  VkViewport opaqueViewport { 0.f,
                              0.f,
                              float(_opaqueMapExtent.x),
                              float(_opaqueMapExtent.y),
                              0.f,
                              1.f};
  VkRect2D opaqueScissor {0,
                          0,
                          uint32_t(_opaqueMapExtent.x),
                          uint32_t(_opaqueMapExtent.y)};

  DrawPlanBuildInfo opaquePlanBuildInfo(drawPlan);
  opaquePlanBuildInfo.adjustFrameRender(opaqueViewport,
                                        opaqueScissor,
                                        opaqueFramePlan,
                                        renderCamera,
                                        &rootViewInfo);

  VkViewport transparentViewport {0.f,
                                  0.f,
                                  float(_transparentMapExtent.x),
                                  float(_transparentMapExtent.y),
                                  0.f,
                                  1.f};
  VkRect2D transparentScissor { 0,
                                0,
                                uint32_t(_transparentMapExtent.x),
                                uint32_t(_transparentMapExtent.y)};

  DrawPlanBuildInfo transparentPlanBuildInfo(drawPlan);
  transparentPlanBuildInfo.adjustFrameRender( transparentViewport,
                                              transparentScissor,
                                              transparentFramePlan,
                                              renderCamera,
                                              &rootViewInfo);

  ViewFrustum localFrustum = opaquePlanBuildInfo.viewFrustum;
  localFrustum.fastTranslate(
            glm::transpose(opaquePlanBuildInfo.drawMatrices.localToViewMatrix));

  DrawVisitor drawVisitor({&opaquePlanBuildInfo, &transparentPlanBuildInfo},
                          localFrustum);
  _targetField->pass(drawVisitor, localFrustum);
}
