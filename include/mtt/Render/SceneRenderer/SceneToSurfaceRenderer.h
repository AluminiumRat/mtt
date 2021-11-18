#pragma once

#include <memory>
#include <vector>

#include <mtt/Render/CommandQueue/CommandQueue.h>
#include <mtt/Render/CommandQueue/Semaphore.h>
#include <mtt/Render/SceneRenderer/AbstractSceneRenderer.h>
#include <mtt/Render/Ref.h>
#include <mtt/Render/RenderSurface.h>
#include <mtt/Render/SwapChain.h>

namespace mtt
{
  class AbstractColorFrameBuilder;
  class CameraNode;
  class LogicalDevice;
  class RenderScene;

  class SceneToSurfaceRenderer : public AbstractSceneRenderer
  {
  public:
    SceneToSurfaceRenderer( LogicalDevice& device,
                            CommandQueue& drawQueue,
                            CommandQueue& presentationQueue,
                            RenderSurface& surface);
    SceneToSurfaceRenderer(const SceneToSurfaceRenderer&) = delete;
    SceneToSurfaceRenderer& operator = (const SceneToSurfaceRenderer&) = delete;
    virtual ~SceneToSurfaceRenderer() noexcept = default;

    virtual void render(RenderScene& scene,
                        const CameraNode& camera,
                        ViewInfo* rootViewInfo) override;

    inline VkFormat targetImageFormat() const noexcept;

    inline AbstractColorFrameBuilder* frameBuilder() const noexcept;
    void setFrameBuilder(AbstractColorFrameBuilder* newBuilder);

  private:
    void _clearFrames() noexcept;
    void _clearSwapchain() noexcept;
    void _rebuildSwapchain();
    void _createFrames();

  private:
    LogicalDevice& _device;
    CommandQueue& _drawQueue;
    CommandQueue& _presentationQueue;
    RenderSurface& _surface;
    Ref<SwapChain> _swapChain;
    VkFormat _targetImageFormat;

    AbstractColorFrameBuilder* _frameBuilder;

    std::vector<std::unique_ptr<AbstractFrame>> _frames;
    std::vector<Ref<Semaphore>> _startCommandSemaphores;
    std::vector<Ref<Semaphore>> _startPresentSemaphores;
    size_t _frameCounter;
  };

  inline VkFormat SceneToSurfaceRenderer::targetImageFormat() const noexcept
  {
    return _targetImageFormat;
  }

  inline AbstractColorFrameBuilder*
                          SceneToSurfaceRenderer::frameBuilder() const noexcept
  {
    return _frameBuilder;
  }
}