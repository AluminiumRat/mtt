#pragma once

#include <stdexcept>

#include <vulkan/vulkan.h>

#include <mtt/render/CommandQueue/CommandPool.h>
#include <mtt/render/CommandQueue/ScaledVolatileDescriptorPool.h>
#include <mtt/render/CommandQueue/UniformMemoryPool.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/Log.h>

namespace mtt
{
  class Fence;
  class Semaphore;

  /// Abstract class for filling command buffers
  class CommandProducer
  {
  public:
    CommandProducer(CommandPool& pool, VkCommandBufferUsageFlags flags);
    CommandProducer(const CommandProducer&) = delete;
    CommandProducer& operator = (const CommandProducer&) = delete;
    virtual ~CommandProducer() noexcept;

    /// Submit current command buffer and start new buffer
    void startNewBuffer();
    void submitCurrentBuffer( Semaphore* startSemaphore,
                              VkPipelineStageFlags dstStageMask,
                              Semaphore* endSignalSemaphore,
                              Fence* endSignalFence);

    inline VkCommandBuffer bufferHandle() const noexcept;
    inline void lockResource(Lockable& resource);

    /// Command buffer should support uniforms
    template <typename DataType>
    inline UniformMemoryPool::MemoryInfo writeUniformData(const DataType& data);

    /// Result descriptor set will be returned to the pool in the next frame
    inline VkDescriptorSet allocateVolatileSet(
                                    VkDescriptorSetLayout layout,
                                    const DescriptorCounter& descriptorsNumber);

  private:
    void _releaseCurrentBuffer() noexcept;

  private:
    VkCommandBufferUsageFlags _flags;
    CommandPool& _pool;
    CommandBuffer* _currentBuffer;
    VkCommandBuffer _bufferHandle;
    UniformMemoryPool* _uniformMemoryPool;
    ScaledVolatileDescriptorPool* _volatileDescriptorPool;
  };

  inline VkCommandBuffer CommandProducer::bufferHandle() const noexcept
  {
    return _bufferHandle;
  }

  inline void CommandProducer::lockResource(Lockable& resource)
  {
    if(_currentBuffer == nullptr) Abort("CommandProducer::lockResource: command buffer is not started.");
    _currentBuffer->lockResource(resource);
  }

  template <typename DataType>
  inline UniformMemoryPool::MemoryInfo CommandProducer::writeUniformData(
                                                          const DataType& data)
  {
    if(_currentBuffer == nullptr) Abort("CommandProducer::lockResource: command buffer is not started.");
    if(_uniformMemoryPool == nullptr) Abort("CommandProducer::writeUniformData: uniforms writing is not supported.");
    return _uniformMemoryPool->write(data);
  }

  inline VkDescriptorSet CommandProducer::allocateVolatileSet(
                                    VkDescriptorSetLayout layout,
                                    const DescriptorCounter& descriptorsNumber)
  {
    if(_currentBuffer == nullptr) Abort("CommandProducer::lockResource: command buffer is not started.");
    if(_volatileDescriptorPool == nullptr) Abort("CommandProducer::allocateVolatileSet: uniforms writing is not supported.");
    return _volatileDescriptorPool->allocateDescriptorSet(layout,
                                                          descriptorsNumber);
  }
}