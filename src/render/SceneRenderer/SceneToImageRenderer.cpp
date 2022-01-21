#include <mtt/render/CommandQueue/CommandQueue.h>
#include <mtt/render/DrawPlan/AbstractFramePlan.h>
#include <mtt/render/DrawPlan/DrawPlan.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/DrawVisitor.h>
#include <mtt/render/SceneRenderer/SceneToImageRenderer.h>
#include <mtt/render/RenderScene.h>

using namespace mtt;

SceneToImageRenderer::SceneToImageRenderer( OneTargetFrameBuilder& frameBuilder,
                                            CommandQueue& drawQueue) :
  _drawQueue(drawQueue),
  _frameBuilder(frameBuilder),
  _frameExtent(256, 256)
{
}

void SceneToImageRenderer::setExtent(const glm::uvec2& newExtent) noexcept
{
  if(_frameExtent == newExtent) return;
  _frameExtent = newExtent;
  _frame.reset();
}

void SceneToImageRenderer::render(RenderScene& scene,
                                  const CameraNode& camera,
                                  ViewInfo* rootViewInfo)
{
  if(_frame == nullptr) _frame = _frameBuilder.createFrame(_frameExtent);

  VkViewport viewport { 0.f,
                        0.f,
                        float(_frameExtent.x),
                        float(_frameExtent.y),
                        0.f,
                        1.f};
  VkRect2D scissor {0, 0, _frameExtent.x, _frameExtent.y};

  DrawPlan drawPlan;
  DrawPlanBuildInfo planBuildInfo(drawPlan);

  std::unique_ptr<AbstractFramePlan> framePlanPtr =
                                        _frameBuilder.createFramePlan(*_frame);
  AbstractFramePlan& framePlan = *framePlanPtr;
  drawPlan.addFramePlan(std::move(framePlanPtr));

  planBuildInfo.adjustFrameRender(viewport,
                                  scissor,
                                  framePlan,
                                  camera,
                                  rootViewInfo);

  DrawVisitor drawVisitor(planBuildInfo);
  ViewFrustum localFrustum = planBuildInfo.viewFrustum;
  localFrustum.fastTranslate(
                  glm::transpose(planBuildInfo.drawMatrices.localToViewMatrix));
  scene.culledData().pass(drawVisitor, localFrustum);
  scene.unculledData().addToDrawPlan(planBuildInfo);
  scene.hudData().addToDrawPlan(planBuildInfo);

  std::unique_ptr<CommandProducer> drawProducer = _drawQueue.startCommands();

  drawPlan.scheduleRender(*drawProducer);

  Fence& waitFence = _drawQueue.submit(
                                std::move(drawProducer),
                                nullptr,
                                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                nullptr);
  waitFence.wait();
}
