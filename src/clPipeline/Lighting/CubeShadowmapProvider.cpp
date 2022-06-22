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
                                              glm::uvec2 frameExtent,
                                              LogicalDevice& device) :
  _frameExtent(frameExtent),
  _frameBuilder(shadowmapFormat, shadowmapLayout, device),
  _framePools(framePoolsNumber),
  _currentPoolIndex(0),
  _targetField(nullptr)
{
  if(framePoolsNumber == 0) Abort("CubeShadowmapProvider::CubeShadowmapProvider: framePoolsNumber = 0");
  if(frameExtent.x == 0 || frameExtent.y == 0) Abort("CubeShadowmapProvider::CubeShadowmapProvider: wrong frame extent.");
}

void CubeShadowmapProvider::setFrameExtent(glm::uvec2 frameExtent)
{
  if (frameExtent.x == 0 || frameExtent.y == 0) Abort("CubeShadowmapProvider::setFrameExtent: wrong frame extent.");

  for(FramePool& pool : _framePools) pool.clear();
  _frameExtent = frameExtent;
}

void CubeShadowmapProvider::setTargetField(AbstractField* newField) noexcept
{
  _targetField = newField;
}

ImageView& CubeShadowmapProvider::getShadowMap( const CameraNode& frontCamera,
                                                DrawPlanBuildInfo& buildInfo)
{
  DrawPlan& drawPlan = buildInfo.drawPlan;

  std::vector<AbstractFramePlan*> existingPlans =
                                        drawPlan.findFramePlans(_frameBuilder);
  ImageView* existingMap = _tryUseExistingPlan( existingPlans,
                                                frontCamera,
                                                drawPlan,
                                                *buildInfo.currentFramePlan);
  if(existingMap != nullptr) return *existingMap;

  size_t frameIndex = existingPlans.size() / 6;
  ShadowmapRecord& shadowmapRecord = _getOrCreateBlankShadowmap(frameIndex);

  _buildNewMap( frontCamera,
                shadowmapRecord,
                drawPlan,
                *buildInfo.currentFramePlan,
                buildInfo.rootViewInfo);

  return *shadowmapRecord.samplerImageView;
}

ImageView* CubeShadowmapProvider::_tryUseExistingPlan(
                                  const std::vector<AbstractFramePlan*>& plans,
                                  const CameraNode& frontCamera,
                                  DrawPlan& drawPlan,
                                  const AbstractFramePlan& dependentFrame)
{
  glm::mat4 viewProjectionMatrix =
                      frontCamera.projectionMatrix() * frontCamera.viewMatrix();

  for (AbstractFramePlan* framePlan : plans)
  {
    ShadowmapBuilder::ShadowMapFramePlan& shadowmapPlan =
                static_cast<ShadowmapBuilder::ShadowMapFramePlan&>(*framePlan);
    if (shadowmapPlan.viewProjectionMatrix == viewProjectionMatrix)
    {
      for (ShadowmapRecord& frameRecord : _framePools[_currentPoolIndex])
      {
        if (frameRecord.frontFrame.get() == &shadowmapPlan.frame())
        {
          drawPlan.addDependency(*framePlan, dependentFrame);
          return frameRecord.samplerImageView.get();
        }
      }
    }
  }
  return nullptr;
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
    mtt::Ref<Image> targetImage(new Image(VK_IMAGE_TYPE_2D,
                                          shadowmapLayout,
                                          shadowmapUsage,
                                          VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
                                          shadowmapFormat,
                                          glm::uvec3(_frameExtent, 1),
                                          VK_SAMPLE_COUNT_1_BIT,
                                          6,
                                          1,
                                          VK_IMAGE_ASPECT_COLOR_BIT,
                                          _frameBuilder.device()));

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

    Ref<ImageView> samplerView(new ImageView( *targetImage,
                                              VK_IMAGE_VIEW_TYPE_CUBE,
                                              colorMapping,
                                              subresourceRange));

    ShadowmapRecord newRecord;
    newRecord.rightFrame = _createFrame(*targetImage,0);
    newRecord.leftFrame = _createFrame(*targetImage, 1);
    newRecord.upFrame = _createFrame(*targetImage, 2);
    newRecord.bottomFrame = _createFrame(*targetImage, 3);
    newRecord.frontFrame = _createFrame(*targetImage, 4);
    newRecord.backFrame = _createFrame(*targetImage, 5);
    newRecord.samplerImageView = samplerView;
    pool.push_back(std::move(newRecord));
  }

  return pool[index];
}

std::unique_ptr<AbstractFrame> CubeShadowmapProvider::_createFrame(
                                                                Image& target,
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

  Ref<ImageView> targetView(new ImageView(target,
                                          VK_IMAGE_VIEW_TYPE_2D,
                                          colorMapping,
                                          subresourceRange));
  return _frameBuilder.createFrame(*targetView);
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
                  *shadowmapRecord.rightFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);

  renderCamera.setTransformMatrix(frontCamera.transformMatrix() *
                                        glm::rotate(glm::pi<float>() / 2.f,
                                                    glm::vec3(0.f, 1.f, 0.f)));
  _buildFramePlan(renderCamera,
                  *shadowmapRecord.leftFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);

  renderCamera.setTransformMatrix(frontCamera.transformMatrix() *
                                        glm::rotate(glm::pi<float>() / 2.f,
                                                    glm::vec3(1.f, 0.f, 0.f)));
  _buildFramePlan(renderCamera,
                  *shadowmapRecord.upFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);

  renderCamera.setTransformMatrix(frontCamera.transformMatrix() *
                                        glm::rotate(-glm::pi<float>() / 2.f,
                                                    glm::vec3(1.f, 0.f, 0.f)));
  _buildFramePlan(renderCamera,
                  *shadowmapRecord.bottomFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);

  renderCamera.setTransformMatrix(frontCamera.transformMatrix());
  _buildFramePlan(renderCamera,
                  *shadowmapRecord.frontFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);

  renderCamera.setTransformMatrix(frontCamera.transformMatrix() *
                                        glm::rotate(-glm::pi<float>(),
                                                    glm::vec3(0.f, 1.f, 0.f)));
  _buildFramePlan(renderCamera,
                  *shadowmapRecord.backFrame,
                  drawPlan,
                  dependentFrame,
                  rootViewInfo);
}

void CubeShadowmapProvider::_buildFramePlan(
                                        const CameraNode& renderCamera,
                                        AbstractFrame& frame,
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
