#include <stdexcept>

#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/render/PhysicalDevice.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

Sampler::SamplerHandle::~SamplerHandle() noexcept
{
  if(value != VK_NULL_HANDLE)
  {
    vkDestroySampler(device, value, nullptr);
    value = VK_NULL_HANDLE;
  }
}

Sampler::Sampler( size_t arraySize,
                  PipelineResource::Type type,
                  LogicalDevice& device)  noexcept :
  PipelineResource( type,
                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    uint32_t(arraySize)),
  _device(device),
  _minFilter(VK_FILTER_LINEAR),
  _magFilter(VK_FILTER_LINEAR),
  _mipmapMode(VK_SAMPLER_MIPMAP_MODE_LINEAR),
  _mipLodBias(0),
  _minLod(0),
  _maxLod(255),
  _addressModeU(VK_SAMPLER_ADDRESS_MODE_REPEAT),
  _addressModeV(VK_SAMPLER_ADDRESS_MODE_REPEAT),
  _addressModeW(VK_SAMPLER_ADDRESS_MODE_REPEAT),
  _maxAnisotropy(1),
  _compareEnable(false),
  _compareOp(VK_COMPARE_OP_ALWAYS),
  _borderColor(VK_BORDER_COLOR_INT_OPAQUE_BLACK),
  _unnormalizedCoordinatesEnable(false),
  _attachedTextures(arraySize),
  _samplerRevision(0),
  _lastTextureRevisions(arraySize)
{
  for(size_t& revision : _lastTextureRevisions) revision = 0;
}

Sampler::Sampler( PipelineResource::Type type,
                  LogicalDevice& device)  noexcept :
  PipelineResource(type, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1),
  _device(device),
  _minFilter(VK_FILTER_LINEAR),
  _magFilter(VK_FILTER_LINEAR),
  _mipmapMode(VK_SAMPLER_MIPMAP_MODE_LINEAR),
  _mipLodBias(0),
  _minLod(0),
  _maxLod(255),
  _addressModeU(VK_SAMPLER_ADDRESS_MODE_REPEAT),
  _addressModeV(VK_SAMPLER_ADDRESS_MODE_REPEAT),
  _addressModeW(VK_SAMPLER_ADDRESS_MODE_REPEAT),
  _maxAnisotropy(1),
  _compareEnable(false),
  _compareOp(VK_COMPARE_OP_ALWAYS),
  _borderColor(VK_BORDER_COLOR_INT_OPAQUE_BLACK),
  _unnormalizedCoordinatesEnable(false),
  _attachedTextures(1),
  _samplerRevision(0),
  _lastTextureRevisions(1)
{
  _lastTextureRevisions[0] = 0;
}

void Sampler::prepareToBind(CommandProducer& drawProducer)
{
  for(std::shared_ptr<AbstractTexture>& texture : _attachedTextures)
  {
    if (texture == nullptr) Abort("Sampler::prepareToBind: attached texture is null.");
    texture->lockResources(drawProducer);
  }
  
  if (_samplerHandle == nullptr) _buildSampler();
  drawProducer.lockResource(*_samplerHandle);
}

void Sampler::_buildSampler()
{
  Ref<SamplerHandle> newHandle(new SamplerHandle);
  newHandle->device = _device.handle();

  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  
  samplerInfo.minFilter = _minFilter;
  samplerInfo.magFilter = _magFilter;
  samplerInfo.mipmapMode = _mipmapMode;
  samplerInfo.mipLodBias = _mipLodBias;
  samplerInfo.minLod = _minLod;
  samplerInfo.maxLod = _maxLod;

  samplerInfo.addressModeU = _addressModeU;
  samplerInfo.addressModeV = _addressModeV;
  samplerInfo.addressModeW = _addressModeW;

  VkBool32 anisotropyEnable = _maxAnisotropy > 1.f ? VK_TRUE : VK_FALSE;
  if(_device.features().samplerAnisotropy == VK_FALSE)
  {
    anisotropyEnable = VK_FALSE;
  }
  samplerInfo.anisotropyEnable = anisotropyEnable;
  samplerInfo.maxAnisotropy =
        std::min(
            _maxAnisotropy,
            _device.physicalDevice().properties().limits.maxSamplerAnisotropy);

  samplerInfo.borderColor = _borderColor;

  samplerInfo.unnormalizedCoordinates = _unnormalizedCoordinatesEnable ?
                                                                      VK_TRUE :
                                                                      VK_FALSE;

  samplerInfo.compareEnable = _compareEnable ? VK_TRUE : VK_FALSE;
  samplerInfo.compareOp = _compareOp;

  if(vkCreateSampler( _device.handle(),
                      &samplerInfo,
                      nullptr,
                      &newHandle->value) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create texture sampler.");
  }

  _samplerHandle = std::move(newHandle);
}

void Sampler::updateDescriptor( VkDescriptorSet descriptorSet,
                                uint32_t binding,
                                CommandProducer& drawProducer,
                                LogicalDevice& device)
{
  if(_samplerHandle == nullptr) Abort("Sampler::updateDescriptor: sampler handle is null.");

  for(uint32_t arrayIndex = 0;
      arrayIndex < _attachedTextures.size();
      arrayIndex++)
  {
    AbstractTexture* texture = _attachedTextures[arrayIndex].get();
    if(texture == nullptr) Abort("Sampler::updateDescriptor: attached texture is null.");
    texture->updateDescriptor(descriptorSet,
                              binding,
                              arrayIndex,
                              _samplerHandle->value);
  }
}

void Sampler::setAttachedTexture( std::shared_ptr<AbstractTexture> texture,
                                  size_t arrayIndex) noexcept
{
  AbstractTexture* newTexture = texture.get();
  _attachedTextures[arrayIndex] = std::move(texture);
  _samplerRevision++;
  if(newTexture != nullptr)
  {
    _lastTextureRevisions[arrayIndex] = newTexture->revision();
  }
}

size_t Sampler::revision() const noexcept
{
  for(size_t arrayIndex = 0;
      arrayIndex < _attachedTextures.size();
      arrayIndex++)
  {
    if (_attachedTextures[arrayIndex] != nullptr &&
        _lastTextureRevisions[arrayIndex] !=
                                      _attachedTextures[arrayIndex]->revision())
    {
      _samplerRevision++;
      _lastTextureRevisions[arrayIndex] =
                                      _attachedTextures[arrayIndex]->revision();
    }
  }

  return _samplerRevision;
}
