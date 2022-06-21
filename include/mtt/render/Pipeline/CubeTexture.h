#pragma once

#include <memory>

#include <vulkan/vulkan.h>

#include <mtt/render/Pipeline/AbstractTexture.h>
#include <mtt/render/Image.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class CubeTexture : public AbstractTexture
  {
  public:
    explicit CubeTexture(LogicalDevice& device) noexcept;
    CubeTexture(const CubeTexture&) = delete;
    CubeTexture& operator = (const CubeTexture&) = delete;
    virtual ~CubeTexture()  noexcept = default;

    struct SideDataInfo
    {
      const void* data = nullptr;
      size_t datasize = 0;
      uint32_t srcRowLength = 0;
    };
    void setData( VkFormat dataFormat,
                  uint32_t extent,
                  VkSampleCountFlagBits samples,
                  const SideDataInfo dataInfo[6],
                  bool generateLods);

    /// Setup external image view.
    /// newImageView should have VK_IMAGE_VIEW_TYPE_CUBE type and
    /// Image must be in VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL during
    /// pipeline binding
    void setImageView(ImageView& newImageView);

    inline VkFormat dataFormat() const noexcept;
    inline uint32_t sideExtent() const noexcept;
    inline VkSampleCountFlagBits samples() const noexcept;
    inline uint32_t lodCount() const noexcept;

    void buildDiffuseLuminanceMap(
                                std::shared_ptr<CubeTexture> environmentTexture,
                                uint32_t extent);

  private:
    Ref<Image> _buildImage( VkFormat dataFormat,
                            uint32_t sideExtent,
                            VkSampleCountFlagBits samples,
                            uint32_t lodCount,
                            VkImageLayout layout,
                            VkImageUsageFlags usageFlags);
    Ref<ImageView> _buildImageView(Image& image);

    void _setSideData(Image& image,
                      uint32_t side,
                      SideDataInfo dataInfo,
                      uint32_t sideExtent);

  private:
    VkFormat _dataFormat;
    uint32_t _sideExtent;
    VkSampleCountFlagBits _samples;
    uint32_t _lodCount;
  };

  inline VkFormat CubeTexture::dataFormat() const noexcept
  {
    return _dataFormat;
  }

  inline uint32_t CubeTexture::sideExtent() const noexcept
  {
    return _sideExtent;
  }

  inline VkSampleCountFlagBits CubeTexture::samples() const noexcept
  {
    return _samples;
  }

  inline uint32_t CubeTexture::lodCount() const noexcept
  {
    return _lodCount;
  }
}