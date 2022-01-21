#pragma once

#include <vulkan/vulkan.h>

#include <glm/vec2.hpp>

#include <mtt/render/Pipeline/AbstractTexture.h>
#include <mtt/render/Image.h>

namespace mtt
{
  class Texture2D : public AbstractTexture
  {
  public:
    explicit Texture2D(LogicalDevice& device) noexcept;
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator = (const Texture2D&) = delete;
    virtual ~Texture2D()  noexcept = default;

    /// Create empty 2D texture
    void makeEmpty( VkFormat dataFormat,
                    const glm::uvec2& extent,
                    VkSampleCountFlagBits samples,
                    uint32_t lodCount);

    /// Create 2D texture from data
    void setData( const void* data,
                  size_t datasize,
                  VkFormat dataFormat,
                  const glm::uvec2& extent,
                  VkSampleCountFlagBits samples,
                  uint32_t srcRowLength,
                  bool generateLods);

    /// Setup external image. Image should have VK_IMAGE_TYPE_2D type.
    /// Image must be in VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL during
    /// pipeline binding
    void setImage(Image& newImage, VkImageAspectFlags dataAspect);

    /// Setup external image view.
    /// newImageView should have VK_IMAGE_VIEW_TYPE_2D type and
    /// Image must be in VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL during
    /// pipeline binding
    void setImageView(ImageView& newImageView);

    inline VkFormat dataFormat() const noexcept;
    inline const glm::uvec2& extent() const noexcept;
    inline VkSampleCountFlagBits samples() const noexcept;
    inline uint32_t lodCount() const noexcept;

  private:
    Ref<Image> _buildImage( const void* data,
                            size_t datasize,
                            uint32_t srcRowLength,
                            VkFormat dataFormat,
                            const glm::uvec2& extent,
                            VkSampleCountFlagBits samples,
                            uint32_t lodCount);
    Ref<ImageView> _buildImageView(Image& image, VkImageAspectFlags dataAspect);

  private:
    VkFormat _dataFormat;
    glm::uvec2 _extent;
    VkSampleCountFlagBits _samples;
    uint32_t _lodCount;
  };

  inline VkFormat Texture2D::dataFormat() const noexcept
  {
    return _dataFormat;
  }

  inline const glm::uvec2& Texture2D::extent() const noexcept
  {
    return _extent;
  }

  inline VkSampleCountFlagBits Texture2D::samples() const noexcept
  {
    return _samples;
  }

  inline uint32_t Texture2D::lodCount() const noexcept
  {
    return _lodCount;
  }
}