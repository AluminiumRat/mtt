#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include <mtt/render/CommandQueue/CommandPool.h>
#include <mtt/render/CommandQueue/CommandProducer.h>
#include <mtt/render/CommandQueue/Fence.h>
#include <mtt/render/CommandQueue/QueueFamilyInfo.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class LogicalDevice;
  class Semaphore;

  class CommandQueue
  {
  private:
    friend class LogicalDevice;
    CommandQueue( LogicalDevice& device,
                  uint32_t familyIndex,
                  uint32_t queueIndex,
                  size_t poolsNumber);
  public:
    CommandQueue(const CommandQueue&) = delete;
    CommandQueue& operator = (const CommandQueue&) = delete;
    ~CommandQueue();

    inline VkQueue handle() const;

    inline LogicalDevice& device();
    inline const LogicalDevice& device() const;

    inline const QueueFamilyInfo& familyInfo() const;

    std::unique_ptr<CommandProducer> startCommands();
    /// You should use producer that was created from this queue
    /// Returns fence to wait end of command execution.
    Fence& submit(std::unique_ptr<CommandProducer> commandProducer,
                  Semaphore* startSemaphore,
                  VkPipelineStageFlags dstStageMask,
                  Semaphore* endSignalSemaphore);

    void submit(CommandBuffer& buffer,
                Semaphore* startSemaphore,
                VkPipelineStageFlags dstStageMask,
                Semaphore* endSignalSemaphore,
                Fence* endSignalFence);

    void waitIdle() const;

    inline void lockResource(Lockable& resource);

  private:
    class Producer : public CommandProducer
    {
    public:
      Producer( CommandPool& pool,
                CommandQueue& queue,
                size_t poolIndex);
      Producer(const Producer&) = delete;
      Producer& operator = (const Producer&) = delete;
      virtual ~Producer();

      CommandQueue& _queue;
      size_t _poolIndex;
    };

  private:
    void _cleanup() noexcept;
    void _unlockPool(size_t _poolIndex) noexcept;

  private:
    VkQueue _handle;
    LogicalDevice& _device;

    QueueFamilyInfo _familyInfo;

    struct PoolRecord
    {
      bool _available;
      std::unique_ptr<CommandPool> pool;
      Ref<Fence> fence;

      PoolRecord() = default;
      PoolRecord(const PoolRecord&) = delete;
      PoolRecord(PoolRecord&&) noexcept = default;
      PoolRecord& operator = (const PoolRecord&) = default;
      PoolRecord& operator = (PoolRecord&&) noexcept = default;
      ~PoolRecord() = default;
    };
    using Pools = std::vector<PoolRecord>;
    Pools _pools;

    size_t _nextPoolIndex;

    using Resources = std::vector<LockableReference>;
    Resources _attachedResources;
  };

  inline VkQueue CommandQueue::handle() const
  {
    return _handle;
  }

  inline LogicalDevice& CommandQueue::device()
  {
    return _device;
  }

  inline const LogicalDevice& CommandQueue::device() const
  {
    return _device;
  }

  inline const QueueFamilyInfo& CommandQueue::familyInfo() const
  {
    return _familyInfo;
  }

  inline void CommandQueue::lockResource(Lockable& resource)
  {
    _attachedResources.push_back(LockableReference(&resource));
  }
}