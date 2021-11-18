#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/Render/DrawPlan/AbstractFramePlan.h>
#include <mtt/Render/SceneGraph/CameraNode.h>

using namespace mtt;

DrawPlanBuildInfo::DrawPlanBuildInfo(DrawPlan& theDrawPlan) :
  drawPlan(theDrawPlan),
  currentFramePlan(nullptr),
  frameType(baseFrameTypes::colorFrameType),
  builder(nullptr),
  frame(nullptr),
  selected(false)
{
}

void DrawPlanBuildInfo::adjustFrameRender(const VkViewport& newViewport,
                                          const VkRect2D& newScissor,
                                          AbstractFramePlan& theFramePlan,
                                          const CameraNode& camera,
                                          ViewInfo* newRootViewInfo) noexcept
{
  currentFramePlan = &theFramePlan;
  frame = &theFramePlan.frame();
  builder = &frame->builder();
  frameType = builder->frameType();
  
  viewport = newViewport;
  scissor = newScissor;

  drawMatrices = camera.buildDrawMatrices();
  currentViewInfo = camera.buildViewInfo(frame->extent());
  if(newRootViewInfo != nullptr) rootViewInfo = *newRootViewInfo;
  else rootViewInfo = currentViewInfo;
  viewFrustum = camera.frustum();
}
