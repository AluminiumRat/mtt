#include <optional>

#include <mtt/render/DrawPlan/AbstractFramePlan.h>
#include <mtt/render/DrawPlan/DrawPlan.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/DrawVisitor.h>
#include <mtt/render/SceneRenderer/AbstractColorFrameBuilder.h>
#include <mtt/render/SceneRenderer/SceneToSurfaceRenderer.h>
#include <mtt/render/RenderScene.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

SceneToSurfaceRenderer::SceneToSurfaceRenderer( LogicalDevice& device,
                                                CommandQueue& drawQueue,
                                                CommandQueue& presentationQueue,
                                                RenderSurface& surface) :
  _device(device),
  _drawQueue(drawQueue),
  _presentationQueue(presentationQueue),
  _surface(surface),
  _presentMode(SwapChain::FIFO_PRESENT_MODE),
  _swapChain(new SwapChain( device,
                            surface,
                            _presentationQueue,
                            {&_drawQueue, &_presentationQueue},
                            _presentMode)),
  _targetImageFormat(_swapChain->imageFormat()),
  _frameBuilder(nullptr),
  _frameCounter(0)
{
  _createFrames();
}

void SceneToSurfaceRenderer::_clearFrames() noexcept
{
  _frames.clear();
  _startCommandSemaphores.clear();
  _startPresentSemaphores.clear();
}

void SceneToSurfaceRenderer::_clearSwapchain() noexcept
{
  _clearFrames();
  _swapChain.reset();
}

void SceneToSurfaceRenderer::_rebuildSwapchain()
{
  _clearSwapchain();

  try
  {
    _swapChain = new SwapChain( _device,
                                _surface,
                                _presentationQueue,
                                { &_drawQueue, &_presentationQueue},
                                _presentMode);
    _createFrames();
  }
  catch(...)
  {
    _clearSwapchain();
    throw;
  }
}

void SceneToSurfaceRenderer::setPresentMode(
                                      SwapChain::PresentMode newValue) noexcept
{
  if(_presentMode == newValue) return;
  _presentMode = newValue;
  _clearSwapchain();
}

void SceneToSurfaceRenderer::_createFrames()
{
  if(_frameBuilder == nullptr) return;
  if(_swapChain == nullptr) return;

  for(size_t frameIndex = 0;
      frameIndex < _swapChain->framesCount();
      frameIndex++)
  {
    _startCommandSemaphores.push_back(Ref<Semaphore>(new Semaphore(_device)));
    _startPresentSemaphores.push_back(Ref<Semaphore>(new Semaphore(_device)));
    
    Image& frameImage = _swapChain->frame(frameIndex);
    _frames.push_back(_frameBuilder->createFrame(frameImage));
  }
}

void SceneToSurfaceRenderer::setFrameBuilder(
                                          AbstractColorFrameBuilder* newBuilder)
{
  if(_frameBuilder == newBuilder) return;
  
  _clearFrames();
  
  _frameBuilder = newBuilder;
  try
  {
    _createFrames();
  }
  catch(...)
  {
    _frameBuilder = nullptr;
    _clearFrames();
    throw;
  }
}

void SceneToSurfaceRenderer::render(RenderScene& scene,
                                    const CameraNode& camera,
                                    ViewInfo* rootViewInfo)
{
  if(_frameBuilder == nullptr) return;
  if(_swapChain == nullptr) _rebuildSwapchain();

  size_t frameIndex = _frameCounter % _swapChain->framesCount();
  std::optional<SwapChain::FrameAccess> access;
  try
  {
    access.emplace(*_swapChain, *_startCommandSemaphores[frameIndex]);
  }
  catch(...)
  {
    _clearSwapchain();
    throw;
  }

  AbstractFrame& frame = *_frames[access->lockedFrameIndex()];

  if(_swapChain->extent().x == 0 || _swapChain->extent().y == 0) return;
  VkViewport viewport { 0.f,
                        0.f,
                        float(_swapChain->extent().x),
                        float(_swapChain->extent().y),
                        0.f,
                        1.f};
  VkRect2D scissor {0, 0, _swapChain->extent().x, _swapChain->extent().y};

  DrawPlan drawPlan;
  DrawPlanBuildInfo planBuildInfo(drawPlan);

  std::unique_ptr<AbstractFramePlan> framePlanPtr =
                                          _frameBuilder->createFramePlan(frame);
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

  _drawQueue.submit(std::move(drawProducer),
                    _startCommandSemaphores[frameIndex].get(),
                    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                    _startPresentSemaphores[frameIndex].get());

  access->present(*_startPresentSemaphores[frameIndex]);

  _frameCounter++;
}
