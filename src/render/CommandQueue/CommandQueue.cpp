#include <stdexcept>

#include <mtt/render/CommandQueue/CommandQueue.h>
#include <mtt/render/CommandQueue/Semaphore.h>
#include <mtt/render/CommandQueue/UniformMemoryPool.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/render/PhysicalDevice.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

CommandQueue::Producer::Producer( CommandPool& pool,
                                  CommandQueue& queue,
                                  size_t poolIndex) :
  CommandProducer(pool, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT),
  _queue(queue),
  _poolIndex(poolIndex)
{
}

CommandQueue::Producer::~Producer()
{
  _queue._unlockPool(_poolIndex);
}

CommandQueue::CommandQueue( LogicalDevice& device,
                            uint32_t familyIndex,
                            uint32_t queueIndex,
                            size_t poolsNumber) :
  _handle(VK_NULL_HANDLE),
  _device(device),
  _familyInfo(device.physicalDevice().queuesInfo()[familyIndex]),
  _nextPoolIndex(0)
{
  try
  {
    vkGetDeviceQueue( device.handle(),
                      familyIndex,
                      queueIndex,
                      &_handle);
    if(_handle == VK_NULL_HANDLE) Abort("CommandQueue::CommandQueue: handle is null.");
  
    for(size_t poolIndex = 0; poolIndex < poolsNumber; poolIndex++)
    {
      PoolRecord newBufferRecord;
      newBufferRecord._available = true;
      newBufferRecord.pool.reset(new CommandPool(*this));
      newBufferRecord.fence = new Fence(_device);
      _pools.push_back(std::move(newBufferRecord));
    }
  }
  catch(...)
  {
    _cleanup();
    throw;
  }
}

CommandQueue::~CommandQueue()
{
  _cleanup();
}

void CommandQueue::_cleanup() noexcept
{
  vkQueueWaitIdle(_handle);

  _pools.clear();
  _attachedResources.clear();
}

void CommandQueue::waitIdle() const
{
  if(vkQueueWaitIdle(_handle) != VK_SUCCESS) throw std::runtime_error("CommandQueue: Failed to wait device queue.");
}

std::unique_ptr<CommandProducer> CommandQueue::startCommands()
{
  size_t attemptCount = 0;
  while(!_pools[_nextPoolIndex]._available)
  {
    attemptCount++;
    if(attemptCount == _pools.size()) Abort("CommandQueue::startCommands: unable to find free pool.");
    _nextPoolIndex = (_nextPoolIndex + 1) % _pools.size();
  };
  
  Fence& waitFence = *_pools[_nextPoolIndex].fence;
  waitFence.wait();

  _pools[_nextPoolIndex]._available = false;
  
  CommandPool& pool = *_pools[_nextPoolIndex].pool;
  std::unique_ptr<CommandProducer> producer(new Producer( pool,
                                                          *this,
                                                          _nextPoolIndex));
  _nextPoolIndex = (_nextPoolIndex + 1) % _pools.size();
  return std::move(producer);
}

Fence& CommandQueue::submit(std::unique_ptr<CommandProducer> commandProducer,
                            Semaphore* startSemaphore,
                            VkPipelineStageFlags dstStageMask,
                            Semaphore* endSignalSemaphore)
{
  Producer& producer = static_cast<Producer&>(*commandProducer);
  if(&producer._queue != this) Abort("CommandQueue::submit: command producer was created in a different queue.");

  Fence& waitFence = *_pools[producer._poolIndex].fence;

  if(commandProducer->bufferHandle() == VK_NULL_HANDLE)
  {
    commandProducer->startNewBuffer();
  }
  commandProducer->submitCurrentBuffer( startSemaphore,
                                        dstStageMask,
                                        endSignalSemaphore,
                                        &waitFence);

  return waitFence;
}

void CommandQueue::submit(CommandBuffer& buffer,
                          Semaphore* startSemaphore,
                          VkPipelineStageFlags dstStageMask,
                          Semaphore* endSignalSemaphore,
                          Fence* endSignalFence)
{
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  submitInfo.waitSemaphoreCount = 0;
  VkSemaphore waitSemaphores[] = {VK_NULL_HANDLE};
  if(startSemaphore != nullptr)
  {
    submitInfo.waitSemaphoreCount = 1;
    waitSemaphores[0] = { startSemaphore->handle()};
    buffer.lockResource(*startSemaphore);
  }
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = &dstStageMask;

  submitInfo.commandBufferCount = 1;
  VkCommandBuffer bufferHandle = buffer.handle();
  submitInfo.pCommandBuffers = &bufferHandle;
  VkSemaphore signalSemaphores[] = {VK_NULL_HANDLE};
  submitInfo.signalSemaphoreCount = 0;
  if(endSignalSemaphore != nullptr)
  {
    submitInfo.signalSemaphoreCount = 1;
    signalSemaphores[0] = endSignalSemaphore->handle();
    buffer.lockResource(*endSignalSemaphore);
  }
  submitInfo.pSignalSemaphores = signalSemaphores;

  VkFence fenceHandle = VK_NULL_HANDLE;
  if(endSignalFence != nullptr)
  {
    fenceHandle = endSignalFence->handle();
    endSignalFence->reset();
  }
  
  if (vkQueueSubmit(_handle,
                    1,
                    &submitInfo,
                    fenceHandle) != VK_SUCCESS)
  {
    throw std::runtime_error("CommandQueue: Failed to submit command buffer.");
  }
}

void CommandQueue::_unlockPool(size_t _poolIndex) noexcept
{
  _pools[_poolIndex]._available = true;
}
