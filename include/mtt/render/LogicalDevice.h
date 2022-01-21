#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

#include <mtt/render/CommandQueue/CommandQueue.h>
#include <mtt/render/CommandQueue/CommandProducer.h>
#include <mtt/render/CommandQueue/QueueFamilyInfo.h>
#include <mtt/render/Pipeline/ShaderCache.h>

namespace mtt
{
  class PhysicalDevice;
  class RenderSurface;

  class LogicalDevice
  {
  public:
    /// If presentationTargetExample is nullptr then swapchain extension will be
    /// disabled.
    LogicalDevice(PhysicalDevice& physicalDevice,
                  const VkPhysicalDeviceFeatures& features,
                  RenderSurface* presentationTargetExample);
    LogicalDevice(const LogicalDevice&) = delete;
    LogicalDevice& operator = (const LogicalDevice&) = delete;
    ~LogicalDevice() noexcept;

    inline VkDevice handle() const noexcept;
    inline VmaAllocator allocator() noexcept;

    inline const VkPhysicalDeviceFeatures& features() const noexcept;

    inline PhysicalDevice& physicalDevice() const noexcept;

    inline CommandQueue& drawQueue() noexcept;
    inline const CommandQueue& drawQueue() const noexcept;

    inline CommandQueue* presentationQueue() noexcept;
    inline const CommandQueue* presentationQueue() const noexcept;

    inline ShaderCache& shaderCache() noexcept;
    inline const ShaderCache& shaderCache() const noexcept;

    /// Synchronized access to the separate transfer command queue.
    /// The command will executed on the GPU immediately and control
    /// will returned after execution.
    /// command is callable object with signature void(CommandProducer&)
    template<typename TransferCommand>
    inline void runTransferCommand(TransferCommand command);

    bool isSurfaceSuitable(const RenderSurface& surface) const;

  private:
    void _cleanup() noexcept;
    void _searchFamilies(RenderSurface* presentationTargetExample);
    void _buildQueues(bool presentationEnabled);

  private:
    PhysicalDevice& _physicalDevice;
    VkDevice _handle;

    VmaAllocator _allocator;

    VkPhysicalDeviceFeatures _features;

    size_t _drawFamilyIndex;
    uint32_t _drawQueueIndex;
    CommandQueue* _drawQueue;

    size_t _presentationFamilyIndex;
    uint32_t _presentationQueueIndex;
    CommandQueue* _presentationQueue;

    size_t _transferFamilyIndex;
    uint32_t _transferQueueIndex;
    
    CommandQueue* _transferQueue;
    std::mutex _transferQueueMutex;

    ShaderCache _shaderCache;

    std::vector<std::unique_ptr<CommandQueue>> _queues;
  };

  inline VkDevice LogicalDevice::handle() const noexcept
  {
    return _handle;
  }

  inline VmaAllocator LogicalDevice::allocator() noexcept
  {
    return _allocator;
  }

  inline const VkPhysicalDeviceFeatures& LogicalDevice::features()
                                                                  const noexcept
  {
    return _features;
  }

  inline PhysicalDevice& LogicalDevice::physicalDevice() const noexcept
  {
    return _physicalDevice;
  }

  inline CommandQueue& LogicalDevice::drawQueue() noexcept
  {
    return *_drawQueue;
  }

  inline const CommandQueue& LogicalDevice::drawQueue() const noexcept
  {
    return *_drawQueue;
  }

  inline CommandQueue* LogicalDevice::presentationQueue() noexcept
  {
    return _presentationQueue;
  }

  inline const CommandQueue* LogicalDevice::presentationQueue() const noexcept
  {
    return _presentationQueue;
  }

  inline ShaderCache& LogicalDevice::shaderCache() noexcept
  {
    return _shaderCache;
  }

  inline const ShaderCache& LogicalDevice::shaderCache() const noexcept
  {
    return _shaderCache;
  }

  template<typename TransferCommand>
  inline void LogicalDevice::runTransferCommand(TransferCommand command)
  {
    std::lock_guard lock(_transferQueueMutex);
    std::unique_ptr<CommandProducer> producer = _transferQueue->startCommands();
    command(*producer);
    _transferQueue->submit( std::move(producer),
                            nullptr,
                            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                            nullptr);
    _transferQueue->waitIdle();
  }
}