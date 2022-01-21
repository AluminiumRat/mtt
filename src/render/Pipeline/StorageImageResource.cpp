#include <mtt/render/CommandQueue/CommandProducer.h>
#include <mtt/render/Pipeline/StorageImageResource.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

StorageImageResource::StorageImageResource(PipelineResource::Type type) :
  PipelineResource(type, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,1),
  _revision(0)
{
}

size_t StorageImageResource::revision() const noexcept
{
  return _revision;
}

void StorageImageResource::setAttachedImageView(ImageView* imageView) noexcept
{
  _imageView = imageView;
  _revision++;
}

void StorageImageResource::prepareToBind(CommandProducer& drawProducer)
{
  if(_imageView == nullptr) Abort("StorageImageResource::prepareToBind: no attached image view.");
  if(_imageView->image().primaryLayout() != VK_IMAGE_LAYOUT_GENERAL) Abort("StorageImageResource::prepareToBind: image primary layout is not VK_IMAGE_LAYOUT_GENERAL.");
  drawProducer.lockResource(*_imageView);
}

void StorageImageResource::updateDescriptor(VkDescriptorSet descriptorSet,
                                            uint32_t binding,
                                            CommandProducer& drawProducer,
                                            LogicalDevice& device)
{
  if(_imageView == nullptr) Abort("StorageImageResource::updateDescriptor: no attached image view.");

  VkDescriptorImageInfo imageInfo {};
  imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
  imageInfo.imageView = _imageView->handle();
  imageInfo.sampler = VK_NULL_HANDLE;

  VkWriteDescriptorSet writeInfo{};
  writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  writeInfo.dstSet = descriptorSet;
  writeInfo.dstBinding = binding;
  writeInfo.dstArrayElement = 0;
  writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
  writeInfo.descriptorCount = 1;
  writeInfo.pImageInfo = &imageInfo;

  vkUpdateDescriptorSets(device.handle(), 1, &writeInfo, 0, nullptr);
}
