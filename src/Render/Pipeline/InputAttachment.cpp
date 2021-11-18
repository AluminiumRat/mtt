#include <mtt/Render/CommandQueue/CommandProducer.h>
#include <mtt/Render/Pipeline/InputAttachment.h>
#include <mtt/Render/ImageView.h>
#include <mtt/Render/LogicalDevice.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

InputAttachment::InputAttachment(LogicalDevice& device) noexcept :
  PipelineResource(VOLATILE, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1),
  _device(device),
  _attachment(nullptr)
{
}

void InputAttachment::prepareToBind(CommandProducer& drawProducer)
{
  if (_attachment == nullptr) Abort("InputAttachment::prepareToBind: attachment is null.");
  drawProducer.lockResource(*_attachment);
}

void InputAttachment::updateDescriptor( VkDescriptorSet descriptorSet,
                                        uint32_t binding,
                                        CommandProducer& drawProducer,
                                        LogicalDevice& device)
{
  if(_attachment == nullptr) Abort("InputAttachment::updateDescriptor: attachment is null.");

  VkDescriptorImageInfo imageInfo {};
  imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  imageInfo.imageView = _attachment->handle();
  imageInfo.sampler = VK_NULL_HANDLE;

  VkWriteDescriptorSet writeInfo{};
  writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  writeInfo.dstSet = descriptorSet;
  writeInfo.dstBinding = binding;
  writeInfo.dstArrayElement = 0;
  writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
  writeInfo.descriptorCount = 1;
  writeInfo.pImageInfo = &imageInfo;

  vkUpdateDescriptorSets(_device.handle(), 1, &writeInfo, 0, nullptr);
}

size_t InputAttachment::revision() const noexcept
{
  return 0;
}
