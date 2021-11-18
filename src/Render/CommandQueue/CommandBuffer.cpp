#include <stdexcept>

#include <mtt/Render/CommandQueue/CommandBuffer.h>
#include <mtt/Render/LogicalDevice.h>

using namespace mtt;

CommandBuffer::CommandBuffer( VkCommandPool pool,
                              LogicalDevice& device,
                              Level level) :
  _handle(VK_NULL_HANDLE),
  _pool(pool),
  _device(device),
  _level(level),
  _uniformMemoryPool(256 * 1024, device),
  _volatileDescriptorPool({ 10,   //samplers
                            512,  //combinedImageSamplers
                            10,   //sampledImage
                            10,   //storageImage
                            10,   //uniformTexelBuffers
                            10,   //storageTexelBuffers
                            2048, //uniformBuffers
                            10,   //storageBuffers
                            20 },  //inputAttachments
                          1024,
                          device)
{
  try
  {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _pool;
    allocInfo.level = VkCommandBufferLevel(level);
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers( _device.handle(),
                                  &allocInfo,
                                  &_handle) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to allocate command buffer.");
    }
  }
  catch(...)
  {
    _cleanup();
    throw;
  };
}

CommandBuffer::~CommandBuffer()
{
  _cleanup();
}

void CommandBuffer::_cleanup() noexcept
{
  if(_handle != VK_NULL_HANDLE)
  {
    vkFreeCommandBuffers( _device.handle(),
                          _pool,
                          1,
                          &_handle);
    _handle = VK_NULL_HANDLE;
  }
}
