#include <mtt/Render/Utilities/DescriptorCounter.h>

using namespace mtt;

void DescriptorCounter::add(
                      VkDescriptorType descriptorType, uint32_t value) noexcept
{
  switch(descriptorType)
  {
    case VK_DESCRIPTOR_TYPE_SAMPLER:
      sampers += value;
      break;

    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
      combinedImageSamplers += value;
      break;

    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
      sampledImages += value;
      break;

    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
      storageImages += value;
      break;

    case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
      uniformTexelBuffers += value;
      break;

    case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
      storageTexelBuffers += value;
      break;

    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
      uniformBuffers += value;
      break;

    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
      storageBuffers += value;
      break;

    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
      inputAttachments += value;
      break;
  };
}

void DescriptorCounter::reduce(
                      VkDescriptorType descriptorType, uint32_t value) noexcept
{
  switch(descriptorType)
  {
    case VK_DESCRIPTOR_TYPE_SAMPLER:
      sampers -= value;
      break;

    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
      combinedImageSamplers -= value;
      break;

    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
      sampledImages -= value;
      break;

    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
      storageImages -= value;
      break;

    case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
      uniformTexelBuffers -= value;
      break;

    case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
      storageTexelBuffers -= value;
      break;

    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
      uniformBuffers -= value;
      break;

    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
      storageBuffers -= value;
      break;

    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
      inputAttachments -= value;
      break;
  };
}

std::vector<VkDescriptorPoolSize> DescriptorCounter::makeSizeInfo() const
{
  std::vector<VkDescriptorPoolSize> sizeInfo;
  if (sampers != 0)
  {
    VkDescriptorPoolSize sizeInfoPart{};
    sizeInfoPart.type = VK_DESCRIPTOR_TYPE_SAMPLER;
    sizeInfoPart.descriptorCount = sampers;
    sizeInfo.push_back(sizeInfoPart);
  }
  if (combinedImageSamplers != 0)
  {
    VkDescriptorPoolSize sizeInfoPart{};
    sizeInfoPart.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    sizeInfoPart.descriptorCount = combinedImageSamplers;
    sizeInfo.push_back(sizeInfoPart);
  }
  if (sampledImages != 0)
  {
    VkDescriptorPoolSize sizeInfoPart{};
    sizeInfoPart.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    sizeInfoPart.descriptorCount = sampledImages;
    sizeInfo.push_back(sizeInfoPart);
  }
  if (storageImages != 0)
  {
    VkDescriptorPoolSize sizeInfoPart{};
    sizeInfoPart.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    sizeInfoPart.descriptorCount = storageImages;
    sizeInfo.push_back(sizeInfoPart);
  }
  if (uniformTexelBuffers != 0)
  {
    VkDescriptorPoolSize sizeInfoPart{};
    sizeInfoPart.type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
    sizeInfoPart.descriptorCount = uniformTexelBuffers;
    sizeInfo.push_back(sizeInfoPart);
  }
  if (storageTexelBuffers != 0)
  {
    VkDescriptorPoolSize sizeInfoPart{};
    sizeInfoPart.type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
    sizeInfoPart.descriptorCount = storageTexelBuffers;
    sizeInfo.push_back(sizeInfoPart);
  }
  if (uniformBuffers != 0)
  {
    VkDescriptorPoolSize sizeInfoPart{};
    sizeInfoPart.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    sizeInfoPart.descriptorCount = uniformBuffers;
    sizeInfo.push_back(sizeInfoPart);
  }
  if (storageBuffers != 0)
  {
    VkDescriptorPoolSize sizeInfoPart{};
    sizeInfoPart.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    sizeInfoPart.descriptorCount = storageBuffers;
    sizeInfo.push_back(sizeInfoPart);
  }
  if (inputAttachments != 0)
  {
    VkDescriptorPoolSize sizeInfoPart{};
    sizeInfoPart.type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    sizeInfoPart.descriptorCount = inputAttachments;
    sizeInfo.push_back(sizeInfoPart);
  }
  return sizeInfo;
}
