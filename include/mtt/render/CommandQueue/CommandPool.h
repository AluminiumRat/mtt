#pragma once

#include <vector>

#include <mtt/render/CommandQueue/CommandBuffer.h>

namespace mtt
{
  class CommandQueue;
  class LogicalDevice;

  class CommandPool
  {
  private:
    friend class CommandQueue;
    CommandPool(CommandQueue& queue);
    CommandPool(const CommandPool&) = delete;
    CommandPool& operator = (const CommandPool&) = delete;

  public:
    virtual ~CommandPool() noexcept;
  
    void reset();
    CommandBuffer& getNextBuffer();
  
    inline CommandQueue& queue() const noexcept;

  private:
    void _cleanup() noexcept;
  
  private:
    VkCommandPool _handle;
    LogicalDevice& _device;
    CommandQueue& _queue;

    using Buffers = std::vector<Ref<CommandBuffer>>;
    Buffers _buffers;
    size_t _nextBuffer;
  };

  inline CommandQueue& CommandPool::queue() const noexcept
  {
    return _queue;
  }
};