#include <mtt/render/CommandQueue/CommandProducer.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

Buffer::Buffer(LogicalDevice& device, Usage usage)  noexcept :
  PipelineResource(STATIC, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
  _device(device),
  _usage(usage),
  _size(1),
  _revision(0)
{
  makeEmpty(_size);
}

void Buffer::makeEmpty(size_t size)
{
  if(size == 0) Abort("Buffer::makeEmpty: size is null.");

  _plainBuffer = _buildBuffer(nullptr, size);
  _size = size;
  incrementRevision();
}

void Buffer::setData(const void* data, size_t dataSize)
{
  if (dataSize == 0) Abort("Buffer::setData: size is null.");
  if (data == nullptr) Abort("Buffer::setData: data is null.");

  _plainBuffer = _buildBuffer(data, dataSize);
  _size = dataSize;
  incrementRevision();
}

static PlainBuffer::Usage translateUsage(Buffer::Usage usage) noexcept
{
  switch (usage)
  {
  case Buffer::VERTEX_BUFFER:
    return PlainBuffer::VERTEX_BUFFER;
  case Buffer::INDICES_BUFFER:
    return PlainBuffer::INDICES_BUFFER;
  case Buffer::UNIFORM_BUFFER:
    return PlainBuffer::UNIFORM_BUFFER;
  }
  Abort("PlainBuffer::translateUsage: undefined usage");
}

Ref<PlainBuffer> Buffer::_buildBuffer(const void* data, size_t dataSize)
{
  Ref<PlainBuffer> newBuffer(new PlainBuffer( _device,
                                              dataSize,
                                              translateUsage(_usage)));
  if(data != nullptr)
  {
    Ref<PlainBuffer> uploadBuffer(new PlainBuffer(_device,
                                                  dataSize,
                                                  PlainBuffer::UPLOAD_BUFFER));
    uploadBuffer->uploadData(data, 0, dataSize);

    _device.runTransferCommand( [&](CommandProducer& uploadProducer)
                                {
                                  newBuffer->copyData(*uploadBuffer,
                                                      0,
                                                      0,
                                                      dataSize,
                                                      uploadProducer);
                                });
  }
  return newBuffer;
}

void Buffer::prepareToBind(CommandProducer& drawProducer)
{
  if (_plainBuffer == nullptr) Abort("Buffer::prepareToBind: main buffer is null.");
  drawProducer.lockResource(*_plainBuffer);
}

void Buffer::updateDescriptor(VkDescriptorSet descriptorSet,
                              uint32_t binding,
                              CommandProducer& drawProducer,
                              LogicalDevice& device)
{
  if(_plainBuffer == nullptr) Abort("Buffer::updateDescriptor: main buffer is null.");

  VkDescriptorBufferInfo bufferInfo{};
  bufferInfo.buffer = _plainBuffer->handle();
  bufferInfo.offset = 0;
  bufferInfo.range = _size;

  VkWriteDescriptorSet descriptorWriteInfo{};
  descriptorWriteInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWriteInfo.dstSet = descriptorSet;
  descriptorWriteInfo.dstBinding = binding;
  descriptorWriteInfo.dstArrayElement = 0;
  descriptorWriteInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorWriteInfo.descriptorCount = 1;
  descriptorWriteInfo.pBufferInfo = &bufferInfo;

  vkUpdateDescriptorSets( _device.handle(),
                          1,
                          &descriptorWriteInfo,
                          0,
                          nullptr);
}

size_t Buffer::revision() const noexcept
{
  return _revision;
}
