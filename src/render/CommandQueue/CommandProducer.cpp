#include <mtt/render/CommandQueue/CommandQueue.h>
#include <mtt/render/CommandQueue/CommandProducer.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

CommandProducer::CommandProducer( CommandPool& pool,
                                  VkCommandBufferUsageFlags flags) :
  _flags(flags),
  _pool(pool),
  _currentBuffer(nullptr),
  _bufferHandle(VK_NULL_HANDLE),
  _uniformMemoryPool(nullptr),
  _volatileDescriptorPool(nullptr)
{
  _pool.reset();
  startNewBuffer();
}

CommandProducer::~CommandProducer()
{
  _releaseCurrentBuffer();
}

void CommandProducer::_releaseCurrentBuffer() noexcept
{
  if(_currentBuffer == nullptr) return;
  if(_uniformMemoryPool != nullptr && _uniformMemoryPool->isStarted())
  {
    _uniformMemoryPool->finish();
    _uniformMemoryPool = nullptr;
  }

  _volatileDescriptorPool = nullptr;

  if(vkEndCommandBuffer(_bufferHandle) != VK_SUCCESS)
  {
    Log() << "Failed to end recording command buffer.";
  }
  _currentBuffer = nullptr;
  _bufferHandle = VK_NULL_HANDLE;
}

void CommandProducer::startNewBuffer()
{
  try
  {
    if(_currentBuffer != nullptr) submitCurrentBuffer(nullptr,
                                                      0,
                                                      nullptr,
                                                      nullptr);

    _currentBuffer = &_pool.getNextBuffer();
    _bufferHandle = _currentBuffer->handle();
  
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = _flags;
    beginInfo.pInheritanceInfo = nullptr;
    if (vkBeginCommandBuffer(_currentBuffer->handle(), &beginInfo) != VK_SUCCESS)
    {
      throw std::runtime_error("CommandProducer: Failed to begin recording command buffer.");
    }

    _uniformMemoryPool = &_currentBuffer->uniformMemoryPool();
    _uniformMemoryPool->start();
    _volatileDescriptorPool = &_currentBuffer->volatileDescriptorPool();
    _volatileDescriptorPool->reset();
  }
  catch(...)
  {
    _releaseCurrentBuffer();
    throw;
  }
}

void CommandProducer::submitCurrentBuffer(Semaphore* startSemaphore,
                                          VkPipelineStageFlags dstStageMask,
                                          Semaphore* endSignalSemaphore,
                                          Fence* endSignalFence)
{
  if(_currentBuffer == nullptr) Abort("CommandProducer::submitCurrentBuffer: command buffer is not started.");

  CommandBuffer& submitBuffer = *_currentBuffer;
  _releaseCurrentBuffer();

  CommandQueue& queue = _pool.queue();
  queue.submit( submitBuffer,
                startSemaphore,
                dstStageMask,
                endSignalSemaphore,
                endSignalFence);
}
