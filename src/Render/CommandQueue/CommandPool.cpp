#include <stdexcept>

#include <mtt/Render/CommandQueue/CommandBuffer.h>
#include <mtt/Render/CommandQueue/CommandPool.h>
#include <mtt/Render/LogicalDevice.h>

using namespace mtt;

CommandPool::CommandPool(CommandQueue& queue) :
  _handle(VK_NULL_HANDLE),
  _device(queue.device()),
  _queue(queue),
  _nextBuffer(0)
{
  try
  {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queue.familyInfo().index();
    poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

    if (vkCreateCommandPool(_device.handle(),
                            &poolInfo,
                            nullptr,
                            &_handle) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to create command pool.");
    }
  }
  catch(...)
  {
    _cleanup();
    throw;
  }
}

CommandPool::~CommandPool() noexcept
{
  _cleanup();
}

void CommandPool::_cleanup() noexcept
{
  _buffers.clear();

  if(_handle != VK_NULL_HANDLE)
  {
    vkDestroyCommandPool(_device.handle(), _handle, nullptr);
    _handle = VK_NULL_HANDLE;
  }
}

void CommandPool::reset()
{
  for(Ref<CommandBuffer>& buffer : _buffers)
  {
    buffer->releaseResources();
  }

  if(vkResetCommandPool(_device.handle(), _handle, 0) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to reset command pool");
  }
  _nextBuffer = 0;
}

CommandBuffer& CommandPool::getNextBuffer()
{
  if(_buffers.size() <= _nextBuffer)
  {
    Ref<CommandBuffer> newBuffer(new CommandBuffer(
                                                _handle,
                                                _device,
                                                CommandBuffer::PRIMARY_LEVEL));
    _buffers.push_back(std::move(newBuffer));
  }
  return *_buffers[_nextBuffer++];
}
