#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include <mtt/render/CommandQueue/UniformMemoryPool.h>
#include <mtt/render/CommandQueue/ScaledVolatileDescriptorPool.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class UniformMemoryPool;
  class ScaledVolatileDescriptorPool;

  class CommandBuffer : public Lockable
  {
  public:
    enum Level
    {
      PRIMARY_LEVEL = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      SECONDARY_LEVEL = VK_COMMAND_BUFFER_LEVEL_SECONDARY
    };

  public:
    CommandBuffer(VkCommandPool pool,
                  LogicalDevice& device,
                  Level level);
    CommandBuffer(const CommandBuffer&) = delete;
    CommandBuffer& operator = (const CommandBuffer&) = delete;
  protected:
    virtual ~CommandBuffer();

  public:
    inline VkCommandBuffer handle() const;
    inline Level level() const;
    inline void lockResource(Lockable& resource);
    inline void releaseResources();
    inline UniformMemoryPool& uniformMemoryPool() noexcept;
    inline ScaledVolatileDescriptorPool& volatileDescriptorPool() noexcept;

  private:
    void _cleanup() noexcept;

  private:
    VkCommandBuffer _handle;
    VkCommandPool _pool;
    LogicalDevice& _device;
    Level _level;

    std::vector<LockableReference> _lockedResources;

    UniformMemoryPool _uniformMemoryPool;
    ScaledVolatileDescriptorPool _volatileDescriptorPool;
  };

  inline VkCommandBuffer CommandBuffer::handle() const
  {
    return _handle;
  }

  inline CommandBuffer::Level CommandBuffer::level() const
  {
    return _level;
  }

  inline void CommandBuffer::lockResource(Lockable& resource)
  {
    _lockedResources.push_back(LockableReference(&resource));
  }

  inline void CommandBuffer::releaseResources()
  {
    _lockedResources.clear();
  }

  inline UniformMemoryPool& CommandBuffer::uniformMemoryPool() noexcept
  {
    return _uniformMemoryPool;
  }

  inline ScaledVolatileDescriptorPool&
                                CommandBuffer::volatileDescriptorPool() noexcept
  {
    return _volatileDescriptorPool;
  }
}