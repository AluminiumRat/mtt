#include <mtt/Render/CommandQueue/CommandProducer.h>
#include <mtt/Render/Pipeline/AbstractTexture.h>
#include <mtt/Render/LogicalDevice.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

AbstractTexture::AbstractTexture(Type type, LogicalDevice& device) :
  _device(device),
  _type(type),
  _revision(0)
{
}

void AbstractTexture::setView(ImageView* imageView) noexcept
{
  _imageView = imageView;
  incrementRevision();
}

void AbstractTexture::lockResources(CommandProducer& drawProducer)
{
  if(_imageView == nullptr) Abort("Texture2D::lockResources: image view is null.");
  if(_imageView->image().primaryLayout() !=
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) Abort("AbstractTexture::lockResources: image primary layout is not VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL.");
  drawProducer.lockResource(*_imageView);
}

void AbstractTexture::updateDescriptor( VkDescriptorSet descriptorSet,
                                        uint32_t binding,
                                        uint32_t arrayIndex,
                                        VkSampler sampler)
{
  if(_imageView == nullptr) Abort("Texture2D::updateDescriptor: image view is null.");
  if(sampler == VK_NULL_HANDLE) Abort("Texture2D::updateDescriptor: sampler is null.");

  VkDescriptorImageInfo imageInfo {};
  imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  imageInfo.imageView = _imageView->handle();
  imageInfo.sampler = sampler;

  VkWriteDescriptorSet writeInfo{};
  writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  writeInfo.dstSet = descriptorSet;
  writeInfo.dstBinding = binding;
  writeInfo.dstArrayElement = arrayIndex;
  writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  writeInfo.descriptorCount = 1;
  writeInfo.pImageInfo = &imageInfo;

  vkUpdateDescriptorSets(_device.handle(), 1, &writeInfo, 0, nullptr);
}
