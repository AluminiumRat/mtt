#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include <mtt/render/Pipeline/PipelineResource.h>
#include <mtt/render/Pipeline/AbstractTexture.h>
#include <mtt/render/Lockable.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class Sampler : public PipelineResource
  {
  public:
    /// arraySize should not be 0.
    Sampler(size_t arraySize,
            PipelineResource::Type type,
            LogicalDevice& device) noexcept;
    /// Initialize sampler with array size = 1
    Sampler(PipelineResource::Type type,
            LogicalDevice& device) noexcept;
    Sampler(const Sampler&) = delete;
    Sampler& operator = (const Sampler&) = delete;
    virtual ~Sampler()  noexcept = default;

    virtual void prepareToBind(CommandProducer& drawProducer) override;
    virtual void updateDescriptor(VkDescriptorSet descriptorSet,
                                  uint32_t binding,
                                  CommandProducer& drawProducer,
                                  LogicalDevice& device) override;
    virtual size_t revision() const noexcept override;

    inline size_t arraySize() const noexcept;

    void setAttachedTexture(std::shared_ptr<AbstractTexture> texture,
                            size_t arrayIndex = 0) noexcept;
    inline AbstractTexture* attachedTexture(size_t arrayIndex) const noexcept;

    inline VkFilter minFilter() const noexcept;
    inline void setMinFilter(VkFilter newValue);

    inline VkFilter magFilter() const noexcept;
    inline void setMagFilter(VkFilter newValue);

    inline VkSamplerMipmapMode mipmapMode() const noexcept;
    inline void setMipmapMode(VkSamplerMipmapMode newValue);

    inline float mipLodBias() const noexcept;
    inline void setMipLodBias(float newValue);

    inline float minLod() const noexcept;
    inline void setMinLod(float newValue);

    inline float maxLod() const noexcept;
    inline void setMaxLod(float newValue);

    inline VkSamplerAddressMode addressModeU() const noexcept;
    inline void setAddressModeU(VkSamplerAddressMode newValue);

    inline VkSamplerAddressMode addressModeV() const noexcept;
    inline void setAddressModeV(VkSamplerAddressMode newValue);

    inline VkSamplerAddressMode addressModeW() const noexcept;
    inline void setAddressModeW(VkSamplerAddressMode newValue);

    inline float maxAnisotropy() const noexcept;
    inline void setMaxAnisotropy(float newValue);

    inline bool compareEnable() const noexcept;
    inline void setCompareEnable(bool newValue);

    inline VkCompareOp compareOp() const noexcept;
    inline void setCompareOp(VkCompareOp newValue);

    inline VkBorderColor borderColor() const noexcept;
    inline void setBorderColor(VkBorderColor newValue);

    inline bool unnormalizedCoordinatesEnable() const noexcept;
    inline void setUnnormalizedCoordinatesEnable(bool newValue);

  private:
    class SamplerHandle : public Lockable
    {
    public:
      VkSampler value = VK_NULL_HANDLE;
      VkDevice device = VK_NULL_HANDLE;
  
      SamplerHandle()  noexcept = default;
      SamplerHandle(const SamplerHandle&) = delete;
      SamplerHandle& operator = (const SamplerHandle&) = delete;
    protected:
      virtual ~SamplerHandle() noexcept;
    };

  private:
    inline void _invalidateSampler() noexcept;
    void _buildSampler();

  private:
    LogicalDevice& _device;
    Ref<SamplerHandle> _samplerHandle;

    VkFilter _minFilter;
    VkFilter _magFilter;
    VkSamplerMipmapMode _mipmapMode;
    float _mipLodBias;
    float _minLod;
    float _maxLod;
    VkSamplerAddressMode _addressModeU;
    VkSamplerAddressMode _addressModeV;
    VkSamplerAddressMode _addressModeW;
    float _maxAnisotropy;
    bool _compareEnable;
    VkCompareOp _compareOp;
    VkBorderColor _borderColor;
    bool _unnormalizedCoordinatesEnable;

    using Textures = std::vector<std::shared_ptr<AbstractTexture>>;
    Textures _attachedTextures;

    mutable size_t _samplerRevision;
    mutable std::vector<size_t> _lastTextureRevisions;
  };

  inline size_t Sampler::arraySize() const noexcept
  {
    return _attachedTextures.size();
  }

  inline AbstractTexture* Sampler::attachedTexture(
                                              size_t arrayIndex) const noexcept
  {
    return _attachedTextures[arrayIndex].get();
  }

  inline VkFilter Sampler::minFilter() const noexcept
  {
    return _minFilter;
  }

  inline void Sampler::setMinFilter(VkFilter newValue)
  {
    if(_minFilter == newValue) return;
    _minFilter = newValue;
    _invalidateSampler();
  }

  inline VkFilter Sampler::magFilter() const noexcept
  {
    return _magFilter;
  }

  inline void Sampler::setMagFilter(VkFilter newValue)
  {
    if (_magFilter == newValue) return;
    _magFilter = newValue;
    _invalidateSampler();
  }

  inline VkSamplerMipmapMode Sampler::mipmapMode() const noexcept
  {
    return _mipmapMode;
  }

  inline void Sampler::setMipmapMode(VkSamplerMipmapMode newValue)
  {
    if (_mipmapMode == newValue) return;
    _mipmapMode = newValue;
    _invalidateSampler();
  }

  inline float Sampler::mipLodBias() const noexcept
  {
    return _mipLodBias;
  }

  inline void Sampler::setMipLodBias(float newValue)
  {
    if (_mipLodBias == newValue) return;
    _mipLodBias = newValue;
    _invalidateSampler();
  }

  inline float Sampler::minLod() const noexcept
  {
    return _minLod;
  }

  inline void Sampler::setMinLod(float newValue)
  {
    if (_minLod == newValue) return;
    _minLod = newValue;
    _invalidateSampler();
  }

  inline float Sampler::maxLod() const noexcept
  {
    return _maxLod;
  }

  inline void Sampler::setMaxLod(float newValue)
  {
    if (_maxLod == newValue) return;
    _maxLod = newValue;
    _invalidateSampler();
  }

  inline VkSamplerAddressMode Sampler::addressModeU() const noexcept
  {
    return _addressModeU;
  }

  inline void Sampler::setAddressModeU(VkSamplerAddressMode newValue)
  {
    if (_addressModeU == newValue) return;
    _addressModeU = newValue;
    _invalidateSampler();
  }

  inline VkSamplerAddressMode Sampler::addressModeV() const noexcept
  {
    return _addressModeV;
  }

  inline void Sampler::setAddressModeV(VkSamplerAddressMode newValue)
  {
    if (_addressModeV == newValue) return;
    _addressModeV = newValue;
    _invalidateSampler();
  }

  inline VkSamplerAddressMode Sampler::addressModeW() const noexcept
  {
    return _addressModeW;
  }

  inline void Sampler::setAddressModeW(VkSamplerAddressMode newValue)
  {
    if (_addressModeW == newValue) return;
    _addressModeW = newValue;
    _invalidateSampler();
  }

  inline float Sampler::maxAnisotropy() const noexcept
  {
    return _maxAnisotropy;
  }

  inline void Sampler::setMaxAnisotropy(float newValue)
  {
    if (_maxAnisotropy == newValue) return;
    _maxAnisotropy = newValue;
    _invalidateSampler();
  }

  inline bool Sampler::compareEnable() const noexcept
  {
    return _compareEnable;
  }

  inline void Sampler::setCompareEnable(bool newValue)
  {
    if (_compareEnable == newValue) return;
    _compareEnable = newValue;
    _invalidateSampler();
  }

  inline VkCompareOp Sampler::compareOp() const noexcept
  {
    return _compareOp;
  }

  inline void Sampler::setCompareOp(VkCompareOp newValue)
  {
    if (_compareOp == newValue) return;
    _compareOp = newValue;
    _invalidateSampler();
  }

  inline VkBorderColor Sampler::borderColor() const noexcept
  {
    return _borderColor;
  }

  inline void Sampler::setBorderColor(VkBorderColor newValue)
  {
    if (_borderColor == newValue) return;
    _borderColor = newValue;
    _invalidateSampler();
  }

  inline bool Sampler::unnormalizedCoordinatesEnable() const noexcept
  {
    return _unnormalizedCoordinatesEnable;
  }

  inline void Sampler::setUnnormalizedCoordinatesEnable(bool newValue)
  {
    if (_unnormalizedCoordinatesEnable == newValue) return;
    _unnormalizedCoordinatesEnable = newValue;
    _invalidateSampler();
  }

  inline void Sampler::_invalidateSampler() noexcept
  {
    _samplerHandle.reset();
    _samplerRevision++;
  }
}