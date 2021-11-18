#pragma once

#include <vulkan/vulkan.h>

#include <mtt/Render/ImageView.h>
#include <mtt/Render/Ref.h>

namespace mtt
{
  class CommandProducer;

  class AbstractTexture
  {
  public:
    enum Type
    {
      TEXTURE_2D,
      CUBE_TEXTURE
    };

  public:
    explicit AbstractTexture(Type type, LogicalDevice& device);
    AbstractTexture(const AbstractTexture&) = delete;
    AbstractTexture& operator = (const AbstractTexture&) = delete;
    virtual ~AbstractTexture()  noexcept = default;

    inline LogicalDevice& device() const noexcept;

    inline Type type() const noexcept;
    inline size_t revision() const noexcept;
  
    virtual void lockResources(CommandProducer& drawProducer);
    virtual void updateDescriptor(VkDescriptorSet descriptorSet,
                                  uint32_t binding,
                                  uint32_t arrayIndex,
                                  VkSampler sampler);
  protected:
    /// Image must be in VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL during
    /// pipeline binding
    void setView(ImageView* imageView) noexcept;
    inline void incrementRevision() noexcept;

  private:
    LogicalDevice& _device;

    Type _type;
    size_t _revision;

    Ref<ImageView> _imageView;
  };

  inline LogicalDevice& AbstractTexture::device() const noexcept
  {
    return _device;
  }

  inline AbstractTexture::Type AbstractTexture::type() const noexcept
  {
    return _type;
  }

  inline size_t AbstractTexture::revision() const noexcept
  {
    return _revision;
  }

  inline void AbstractTexture::incrementRevision() noexcept
  {
    _revision++;
  }
};