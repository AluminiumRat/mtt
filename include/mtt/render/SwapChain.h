#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#include <glm/vec2.hpp>

#include <mtt/render/CommandQueue/Fence.h>
#include <mtt/render/CommandQueue/Semaphore.h>
#include <mtt/render/Image.h>
#include <mtt/render/Ref.h>
#include <mtt/render/Lockable.h>

namespace mtt
{
  class LogicalDevice;
  class RenderSurface;
  class CommandQueue;

  class SwapChain : public Lockable
  {
  public:
    struct FrameAccess
    {
    public:
      inline FrameAccess( SwapChain& swapChain,
                          Semaphore& imageAvailableSemaphore);
      FrameAccess(const FrameAccess&) = delete;
      FrameAccess& operator = (const FrameAccess&) = delete;
      inline FrameAccess(FrameAccess&& other);
      inline ~FrameAccess() noexcept;

      inline size_t lockedFrameIndex() const noexcept;
      inline Image* image() const noexcept;

      inline void present(Semaphore& endDrawningSemaphore);

    private:
      SwapChain& _swapChain;

      size_t _lockedFrameIndex;
      Image* _image;
    };

  public:
    /// ownerQueues is list of queues who use swap chain's images. This is at
    /// least draw queue and present queue.
    /// ownerQueues can contains duplicates.
    SwapChain(LogicalDevice& device,
              RenderSurface& surface,
              CommandQueue& presentQueue,
              std::vector<CommandQueue const*> ownerQueues);
    SwapChain(const SwapChain&) = delete;
    SwapChain& operator = (const SwapChain&) = delete;
  protected:
    virtual ~SwapChain() noexcept;

  public:
    inline VkSwapchainKHR handle() const noexcept;
  
    inline VkFormat imageFormat() const noexcept;
    inline const glm::uvec2& extent() const noexcept;

    inline size_t framesCount() const noexcept;
    inline Image& frame(size_t frameIndex) noexcept;
    inline const Image& frame(size_t frameIndex) const noexcept;

  private:
    /// returns index of locked frame
    size_t _lockFrame(Semaphore& imageAvailableSemaphore);
    void _unlockFrame(Semaphore& endDrawningSemaphore);
    /// Emergency return frame to swapchain without synchronization.
    void _unlockFrame() noexcept;

  private:
    void _createSwapchain(RenderSurface& surface,
                          const std::vector<CommandQueue const*>& ownerQueues);
    void _createSyncObjects();
    void _cleanup() noexcept;

  private:
    struct FrameRecord
    {
      Ref<Image> image;
      Ref<Fence> endDrawningFence;
      Ref<Semaphore> resetFenceSemaphore;
      Ref<Semaphore> imageAvailableSemaphore; /// This is locker for external
                                              /// semaphore
      Ref<Semaphore> endDrawningSemaphore;    /// This is locker for external
                                              /// semaphore
    };

  private:
    LogicalDevice& _device;
    CommandQueue& _presentQueue;

    VkSwapchainKHR _handle;
    VkFormat _imageFormat;
    glm::uvec2 _extent;

    std::vector<FrameRecord> _frames;
    std::optional<uint32_t> _lockedFrameIndex;
  };

  inline SwapChain::FrameAccess::FrameAccess(
                                            SwapChain& swapChain,
                                            Semaphore& imageAvailableSemaphore):
    _swapChain(swapChain),
    _lockedFrameIndex(0),
    _image(nullptr)
  {
    _lockedFrameIndex = _swapChain._lockFrame(imageAvailableSemaphore);
    _image = &_swapChain.frame(_lockedFrameIndex);
  }

  inline SwapChain::FrameAccess::FrameAccess(FrameAccess&& other) :
    _swapChain(other._swapChain),
    _image(other._image)
  {
    other._image = nullptr;
  }

  inline void SwapChain::FrameAccess::present(Semaphore& endDrawningSemaphore)
  {
    if (_image != nullptr) _swapChain._unlockFrame(endDrawningSemaphore);
    _image = nullptr;
  }

  inline SwapChain::FrameAccess::~FrameAccess()
  {
    if(_image != nullptr)
    {
      _swapChain._unlockFrame();
    }
  }

  inline size_t SwapChain::FrameAccess::lockedFrameIndex() const noexcept
  {
    return _lockedFrameIndex;
  }

  inline Image* SwapChain::FrameAccess::image() const noexcept
  {
    return _image;
  }

  inline VkSwapchainKHR SwapChain::handle() const noexcept
  {
    return _handle;
  }

  inline VkFormat SwapChain::imageFormat() const noexcept
  {
    return _imageFormat;
  }

  inline const glm::uvec2& SwapChain::extent() const noexcept
  {
    return _extent;
  }

  inline size_t SwapChain::framesCount() const noexcept
  {
    return _frames.size();
  }

  Image& SwapChain::frame(size_t frameIndex) noexcept
  {
    return *_frames[frameIndex].image;
  }

  const Image& SwapChain::frame(size_t frameIndex) const noexcept
  {
    return *_frames[frameIndex].image;
  }
}