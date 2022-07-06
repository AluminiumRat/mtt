#include <glm/gtx/transform.hpp>

#include <mtt/clPipeline/Lighting/CubeShadowmapProvider.h>
#include <mtt/render/DrawPlan/DrawPlan.h>
#include <mtt/render/SceneGraph/DrawVisitor.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/clip.h>

using namespace mtt;
using namespace mtt::clPipeline;

static constexpr VkImageUsageFlags shadowmapUsage =
                                            VK_IMAGE_USAGE_SAMPLED_BIT |
                                            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

CubeShadowmapProvider::CubeShadowmapProvider( size_t framePoolsNumber,
                                              uint32_t opaqueMapExtent,
                                              uint32_t transparentMapExtent,
                                              LogicalDevice& device) :
  _opaqueMapExtent(opaqueMapExtent),
  _transparentMapExtent(transparentMapExtent),
  _opaqueFrameBuilder(opaqueMapFormat, shadowmapLayout, device),
  _transparentFrameBuilder(transparentMapFormat, shadowmapLayout, device),
  _framePools(framePoolsNumber),
  _currentPoolIndex(0),
  _targetField(nullptr)
{
  if(framePoolsNumber == 0) Abort("CubeShadowmapProvider::CubeShadowmapProvider: framePoolsNumber = 0");
  if(_opaqueMapExtent == 0) Abort("CubeShadowmapProvider::CubeShadowmapProvider: wrong opaque map extent.");
  if (_transparentMapExtent == 0) Abort("CubeShadowmapProvider::CubeShadowmapProvider: wrong transparent map extent.");
}

void CubeShadowmapProvider::setMapExtent( uint32_t opaqueMapExtent,
                                          uint32_t transparentMapExtent)
{
  if (opaqueMapExtent == 0) Abort("CubeShadowmapProvider::setMapExtent: wrong opaque map extent.");
  if (transparentMapExtent == 0) Abort("CubeShadowmapProvider::setMapExtent: wrong transparent map extent.");

  for(FramePool& pool : _framePools) pool.clear();
  _opaqueMapExtent = opaqueMapExtent;
  _transparentMapExtent = transparentMapExtent;
}

void CubeShadowmapProvider::setTargetField(AbstractField* newField) noexcept
{
  _targetField = newField;
}

CubeShadowmapProvider::Shadowmaps CubeShadowmapProvider::getShadowMaps(
                                                  const CameraNode& frontCamera,
                                                  DrawPlanBuildInfo& buildInfo)
{
  DrawPlan& drawPlan = buildInfo.drawPlan;

  std::vector<AbstractFramePlan*> existingPlans =
                                  drawPlan.findFramePlans(_opaqueFrameBuilder);
  ShadowmapRecord* existingMap = _tryUseExistingPlan(
                                                  existingPlans,
                                                  frontCamera,
                                                  drawPlan,
                                                  *buildInfo.currentFramePlan);
  if(existingMap != nullptr)
  {
    return {existingMap->opaqueMapView.get(),
            existingMap->transparentMapView.get()};
  }

  size_t frameIndex = existingPlans.size() / 6;
  ShadowmapRecord& shadowmapRecord = _getOrCreateBlankShadowmap(frameIndex);

  _buildNewMap( frontCamera,
                shadowmapRecord,
                drawPlan,
                *buildInfo.currentFramePlan,
                buildInfo.rootViewInfo);

  return {shadowmapRecord.opaqueMapView.get(),
          shadowmapRecord.transparentMapView.get()};
}

CubeShadowmapProvider::ShadowmapRecord*
            CubeShadowmapProvider::_tryUseExistingPlan(
                                  const std::vector<AbstractFramePlan*>& plans,
                                  const CameraNode& frontCamera,
                                  DrawPlan& drawPlan,
                                  const AbstractFramePlan& dependentFrame)
{
  glm::mat4 viewProjectionMatrix =
                      frontCamera.projectionMatrix() * frontCamera.viewMatrix();

  for (AbstractFramePlan* framePlan : plans)
  {
    OpaqueShadowmapBuilder::FramePlan& shadowmapPlan =
                static_cast<OpaqueShadowmapBuilder::FramePlan&>(*framePlan);
    if (shadowmapPlan.viewProjectionMatrix == viewProjectionMatrix)
    {
      for (ShadowmapRecord& frameRecord : _framePools[_currentPoolIndex])
      {
        if (frameRecord.frontOpaqueFrame.get() == &shadowmapPlan.frame())
        {
          drawPlan.addDependency(*framePlan, dependentFrame);
          return &frameRecord;
        }
      }
    }
  }
  return nullptr;
}

template<typename Builder>
std::unique_ptr<AbstractFrame>
                          CubeShadowmapProvider::_createFrame(Builder& builder,
                                                              Image& shadowMap,
                                                              uint32_t face)
{
  VkComponentMapping colorMapping;
  colorMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  VkImageSubresourceRange subresourceRange;
  subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  subresourceRange.baseMipLevel = 0;
  subresourceRange.levelCount = 1;
  subresourceRange.baseArrayLayer = face;
  subresourceRange.layerCount = 1;

  Ref<ImageView> targetView(new ImageView(shadowMap,
                                          VK_IMAGE_VIEW_TYPE_2D,
                                          colorMapping,
                                          subresourceRange));
  return builder.createFrame(*targetView);
}

CubeShadowmapProvider::ShadowmapRecord&
                CubeShadowmapProvider::_getOrCreateBlankShadowmap(size_t index)
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
                                        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
                                        opaqueMapFormat,
                                        glm::uvec3( _opaqueMapExtent,
                                                    _opaqueMapExtent,
                                                    1),
                                        VK_SAMPLE_COUNT_1_BIT,
                                        6,
                                        1,
                                        VK_IMAGE_ASPECT_COLOR_BIT,
                                        _opaqueFrameBuilder.device()));

    mtt::Ref<Image> transparentMap(new Image(
                                            VK_IMAGE_TYPE_2D,
                                            shadowmapLayout,
                                            shadowmapUsage,
                                            VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
                                            transparentMapFormat,
                                            glm::uvec3( _transparentMapExtent,
                                                        _transparentMapExtent,
                                                        1),
                                            VK_SAMPLE_COUNT_1_BIT,
                                            6,
                                            1,
                                            VK_IMAGE_ASPECT_COLOR_BIT,
                                            _transparentFrameBuilder.device()));

    ShadowmapRecord newRecord;
    newRecord.rightOpaqueFrame = _createFrame(_opaqueFrameBuilder,
                                              *opaqueMap,
                                              0);
    newRecord.rightTransparentFrame = _createFrame( _transparentFrameBuilder,
                                                    *transparentMap,
                                                    0);

    newRecord.leftOpaqueFrame = _createFrame( _opaqueFrameBuilder ,
                                              *opaqueMap,
                                              1);
    newRecord.leftTransparentFrame = _createFrame(_transparentFrameBuilder ,
                                                  *transparentMap,
                                                  1);

    newRecord.upOpaqueFrame = _createFrame( _opaqueFrameBuilder,
                                            *opaqueMap,
                                            2);
    newRecord.upTransparentFrame = _createFrame(_transparentFrameBuilder,
                                                *transparentMap,
                                                2);

    newRecord.bottomOpaqueFrame = _createFrame( _opaqueFrameBuilder,
                                                *opaqueMap,
                                                3);
    newRecord.bottomTransparentFrame = _createFrame(_transparentFrameBuilder,
                                                    *transparentMap,
                                                    3);

    newRecord.frontOpaqueFrame = _createFrame(_opaqueFrameBuilder,
                                              *opaqueMap,
                                              4);
    newRecord.frontTransparentFrame = _createFrame( _transparentFrameBuilder,
                                                    *transparentMap,
                                                    4);

    newRecord.backOpaqueFrame = _createFrame( _opaqueFrameBuilder,
                                              *opaqueMap,
                                              5);
    newRecord.backTransparentFrame = _createFrame(_transparentFrameBuilder,
                                                  *transparentMap,
                                                  5);

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
    subresourceRange.layerCount = 6;

    Ref<ImageView> opaqueMapView(new ImageView( *opaqueMap,
                                                VK_IMAGE_VIEW_TYPE_CUBE,
                                                colorMapping,
                                                subresourceRange));

    Ref<ImageView> transparentMapView(new ImageView(*transparentMap,
                                                    VK_IMAGE_VIEW_TYPE_CUBE,
                                                    colorMapping,
                                                    subresourceRange));

    newRecord.opaqueMapView = opaqueMapView;
    newRecord.transparentMapView = transparentMapView;
    pool.push_back(std::move(newRecord));
  }

  return pool[index];
}

void CubeShadowmapProvider::_buildNewMap(
                                        const CameraNode& frontCamera,
                                        ShadowmapRecord& shadowmapRecord,
                                        DrawPlan& drawPlan,
                                        const AbstractFramePlan& dependentFrame,
                                        ViewInfo& rootViewInfo)
{
  CameraNode renderCamera;
  renderCamera.setProjectionMatrix(frontCamera.projectionMatrix());
  renderCamera.setTransformMatrix(frontCamera.transformMatrix() *
                                        glm::rotate(-glm::pi<float>() / 2.f,
                                                    glm::vec3(0.f, 1.f, 0.f)));
  _buildFramePlan(renderCamera,
                  *shadowmapRecord.rightOpaqueFrame,
                  *shadowmapRecord.rightTransparentFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);

  renderCamera.setTransformMatrix(frontCamera.transformMatrix() *
                                        glm::rotate(glm::pi<float>() / 2.f,
                                                    glm::vec3(0.f, 1.f, 0.f)));
  _buildFramePlan(renderCamera,
                  *shadowmapRecord.leftOpaqueFrame,
                  *shadowmapRecord.leftTransparentFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);

  renderCamera.setTransformMatrix(frontCamera.transformMatrix() *
                                        glm::rotate(glm::pi<float>() / 2.f,
                                                    glm::vec3(1.f, 0.f, 0.f)));
  _buildFramePlan(renderCamera,
                  *shadowmapRecord.upOpaqueFrame,
                  *shadowmapRecord.upTransparentFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);

  renderCamera.setTransformMatrix(frontCamera.transformMatrix() *
                                        glm::rotate(-glm::pi<float>() / 2.f,
                                                    glm::vec3(1.f, 0.f, 0.f)));
  _buildFramePlan(renderCamera,
                  *shadowmapRecord.bottomOpaqueFrame,
                  *shadowmapRecord.bottomTransparentFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);

  renderCamera.setTransformMatrix(frontCamera.transformMatrix());
  _buildFramePlan(renderCamera,
                  *shadowmapRecord.frontOpaqueFrame,
                  *shadowmapRecord.frontTransparentFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);

  renderCamera.setTransformMatrix(frontCamera.transformMatrix() *
                                        glm::rotate(-glm::pi<float>(),
                                                    glm::vec3(0.f, 1.f, 0.f)));
  _buildFramePlan(renderCamera,
                  *shadowmapRecord.backOpaqueFrame,
                  *shadowmapRecord.backTransparentFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);
}

void CubeShadowmapProvider::_buildFramePlan(
                                        const CameraNode& renderCamera,
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
                              float(_opaqueMapExtent),
                              float(_opaqueMapExtent),
                              0.f,
                              1.f};
  VkRect2D opaqueScissor {0, 0, _opaqueMapExtent, _opaqueMapExtent };

  DrawPlanBuildInfo opaquePlanBuildInfo(drawPlan);
  opaquePlanBuildInfo.adjustFrameRender(opaqueViewport,
                                  opaqueScissor,
                                  opaqueFramePlan,
                                  renderCamera,
                                  &rootViewInfo);

  DrawVisitor opaqueDrawVisitor(opaquePlanBuildInfo);
  ViewFrustum localFrustum = opaquePlanBuildInfo.viewFrustum;
  localFrustum.fastTranslate(
            glm::transpose(opaquePlanBuildInfo.drawMatrices.localToViewMatrix));
  _targetField->pass(opaqueDrawVisitor, localFrustum);

  VkViewport transparentViewport{ 0.f,
                                  0.f,
                                  float(_transparentMapExtent),
                                  float(_transparentMapExtent),
                                  0.f,
                                  1.f };
  VkRect2D transparentScissor{0,
                              0,
                              _transparentMapExtent,
                              _transparentMapExtent };

  DrawPlanBuildInfo transparentPlanBuildInfo(drawPlan);
  transparentPlanBuildInfo.adjustFrameRender( transparentViewport,
                                              transparentScissor,
                                              transparentFramePlan,
                                              renderCamera,
                                              &rootViewInfo);

  DrawVisitor transparentDrawVisitor(transparentPlanBuildInfo);
  _targetField->pass(transparentDrawVisitor, localFrustum);
}
