#pragma once

#include <vulkan/vulkan.h>

#include <mtt/Render/Image.h>
#include <mtt/Render/Lockable.h>
#include <mtt/Render/Ref.h>

namespace mtt
{
  class LogicalDevice;

  class ImageView : public Lockable
  {
  public:
    ImageView(Image& image,
              VkImageViewType viewType,
              const VkComponentMapping& components,
              const VkImageSubresourceRange& subresourceRange);
    ImageView(const ImageView&) = delete;
    ImageView& operator = (const ImageView&) = delete;
  protected:
    virtual ~ImageView();

  public:
    inline VkImageView handle() const noexcept;
    inline Image& image() noexcept;
    inline const Image& image() const noexcept;
    inline VkImageViewType viewType() const noexcept;
    inline const VkComponentMapping& components() const noexcept;
    inline const VkImageSubresourceRange& subresourceRange() const noexcept;

    /// Extent of base level
    inline glm::uvec3 extent() const noexcept;
    inline glm::uvec3 extent(uint32_t mipLevel) const noexcept;

  private:
    void _cleanup() noexcept;

  private:
    VkImageView _handle;
    Ref<Image> _image;
    VkImageViewType _viewType;
    VkComponentMapping _components;
    VkImageSubresourceRange _subresourceRange;
  };

  inline VkImageView ImageView::handle() const noexcept
  {
    return _handle;
  }

  inline Image& ImageView::image() noexcept
  {
    return *_image;
  }

  inline const Image& ImageView::image() const noexcept
  {
    return *_image;
  }

  inline VkImageViewType ImageView::viewType() const noexcept
  {
    return _viewType;
  }

  inline const VkComponentMapping& ImageView::components() const noexcept
  {
    return _components;
  }

  inline const VkImageSubresourceRange&
                                    ImageView::subresourceRange() const noexcept
  {
    return _subresourceRange;
  }

  inline glm::uvec3 ImageView::extent() const noexcept
  {
    return extent(0);
  }

  inline glm::uvec3 ImageView::extent(uint32_t mipLevel) const noexcept
  {
    return _image->extent(_subresourceRange.baseMipLevel + mipLevel);
  }
}