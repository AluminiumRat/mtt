#include <mtt/render/CommandQueue/CommandProducer.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/render/PhysicalDevice.h>
#include <mtt/render/PlainBuffer.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

PlainBuffer::MemoryMapper::MemoryMapper(PlainBuffer& buffer) :
  _buffer(buffer),
  _data(nullptr)
{
  if(vmaMapMemory(_buffer._device.allocator(),
                  _buffer._allocation,
                  &_data) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to map buffer's memory.");
  }
}

PlainBuffer::MemoryMapper::~MemoryMapper()
{
  if(_data != nullptr)
  {
    vmaUnmapMemory(_buffer._device.allocator(), _buffer._allocation);
  }
}

static VkBufferUsageFlags getUsageFlags(PlainBuffer::Usage usage)
{
  switch (usage)
  {
    case PlainBuffer::UPLOAD_BUFFER:
      return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    case PlainBuffer::DOWNLOAD_BUFFER:
      return VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    case PlainBuffer::VERTEX_BUFFER:
      return  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
              VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    case PlainBuffer::INDICES_BUFFER:
      return  VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
              VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    case PlainBuffer::UNIFORM_BUFFER:
      return  VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
              VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    case PlainBuffer::VOLATILE_UNIFORM_BUFFER:
      return  VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  }
  return 0;
};

static VmaMemoryUsage getMemoryUsage(PlainBuffer::Usage usage)
{
  switch (usage)
  {
    case PlainBuffer::UPLOAD_BUFFER:
      return VMA_MEMORY_USAGE_CPU_TO_GPU;

    case PlainBuffer::DOWNLOAD_BUFFER:
      return VMA_MEMORY_USAGE_GPU_TO_CPU;

    case PlainBuffer::VERTEX_BUFFER:
      return  VMA_MEMORY_USAGE_GPU_ONLY;

    case PlainBuffer::INDICES_BUFFER:
      return  VMA_MEMORY_USAGE_GPU_ONLY;

    case PlainBuffer::UNIFORM_BUFFER:
      return  VMA_MEMORY_USAGE_GPU_ONLY;

    case PlainBuffer::VOLATILE_UNIFORM_BUFFER:
      return VMA_MEMORY_USAGE_CPU_TO_GPU;
  }
  return VMA_MEMORY_USAGE_CPU_TO_GPU;
};

PlainBuffer::PlainBuffer( LogicalDevice& device,
                          size_t size,
                          Usage usage) :
  _device(device),
  _handle(VK_NULL_HANDLE),
  _size(size),
  _allocation(VK_NULL_HANDLE)
{
  try
  {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = _size;
    bufferInfo.usage = getUsageFlags(usage);
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocCreateInfo{};
    allocCreateInfo.usage = getMemoryUsage(usage);

    if(vmaCreateBuffer( _device.allocator(),
                        &bufferInfo,
                        &allocCreateInfo,
                        &_handle,
                        &_allocation,
                        nullptr) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to create vertex buffer.");
    }
  }
  catch(...)
  {
    _cleanup();
    throw;
  }
}

PlainBuffer::PlainBuffer( LogicalDevice& device,
                          size_t size,
                          VmaMemoryUsage memoryUsage,
                          VkBufferUsageFlags bufferUsageFlags) :
  _device(device),
  _handle(VK_NULL_HANDLE),
  _size(size),
  _allocation(VK_NULL_HANDLE)
{
  try
  {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = _size;
    bufferInfo.usage = bufferUsageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocCreateInfo{};
    allocCreateInfo.usage = memoryUsage;

    if(vmaCreateBuffer( _device.allocator(),
                        &bufferInfo,
                        &allocCreateInfo,
                        &_handle,
                        &_allocation,
                        nullptr) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to create vertex buffer.");
    }
  }
  catch(...)
  {
    _cleanup();
    throw;
  }
}

PlainBuffer::~PlainBuffer()
{
  _cleanup();
}

void PlainBuffer::_cleanup() noexcept
{
  if(_handle != VK_NULL_HANDLE)
  {
    vmaDestroyBuffer(_device.allocator(), _handle, _allocation);
    _allocation = VK_NULL_HANDLE;
    _handle = VK_NULL_HANDLE;
  }
}

void PlainBuffer::uploadData(const void* data, size_t shift, size_t dataSize)
{
  void* bufferData;
  if(vmaMapMemory(_device.allocator(),
                  _allocation,
                  &bufferData) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to map buffer's memory.");
  }

  memcpy((char*)bufferData + shift, data, dataSize);

  vmaUnmapMemory(_device.allocator(), _allocation);
}

void PlainBuffer::copyData( PlainBuffer& srcBuffer,
                            size_t srcOffset,
                            size_t dstOffset,
                            size_t size,
                            CommandProducer& uploadProducer)
{
  if(size == 0) Abort("PlainBuffer::copyData: size is 0");

  VkBufferCopy copyInfo{};
  copyInfo.srcOffset = srcOffset;
  copyInfo.dstOffset = dstOffset;
  copyInfo.size = size;

  vkCmdCopyBuffer(uploadProducer.bufferHandle(),
                  srcBuffer.handle(),
                  _handle,
                  1,
                  &copyInfo);

  uploadProducer.lockResource(*this);
  uploadProducer.lockResource(srcBuffer);
}
