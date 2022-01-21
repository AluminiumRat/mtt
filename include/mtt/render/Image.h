#pragma once

#include <cmath>
#include <vector>

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <mtt/render/Lockable.h>

namespace mtt
{
  class PlainBuffer;
  class CommandProducer;
  class LogicalDevice;

  class Image : public Lockable
  {
  public:
    /// Build empty image.
    Image(VkImageType imageType,
          VkImageLayout primaryLayout,
          VkImageUsageFlags usageFlags,
          VkImageCreateFlags createFlags,
          VkFormat format,
          glm::uvec3 extent,
          VkSampleCountFlagBits samples,
          uint32_t arraySize,
          uint32_t mipmapCount,
          VkImageAspectFlags initAspectMask,
          LogicalDevice& device);

    /// Build image and upload data.
    /// Warning!!! Usage flag VK_IMAGE_USAGE_TRANSFER_DST_BIT will be added
    /// automatically.
    /// If mipmapCount > 1, then data will be uploaded to the first mip level,
    /// and other levels will be generated from it.
    Image(VkImageType imageType,
          VkImageLayout primaryLayout,
          VkImageUsageFlags usageFlags,
          VkImageCreateFlags createFlags,
          VkFormat format,
          glm::uvec3 extent,
          VkSampleCountFlagBits samples,
          uint32_t mipmapCount,
          VkImageAspectFlags dstAspect,
          const void* data,
          size_t datasize,
          uint32_t srcRowLength,
          uint32_t srcImageHeight,
          LogicalDevice& device);

    /// Initializing by prebuilded image
    Image(VkImage handle,
          VkImageType imageType,
          VkImageLayout primaryLayout,
          VkFormat format,
          glm::uvec3 extent,
          VkSampleCountFlagBits samples,
          uint32_t arraySize,
          uint32_t mipmapCount,
          LogicalDevice& device);
    Image(const Image&) = delete;
    Image& operator = (const Image&) = delete;
  protected:
    virtual ~Image();

  public:
    inline LogicalDevice& device() noexcept;
    inline const LogicalDevice& device() const noexcept;
    inline VkImage handle() const noexcept;
    inline VkImageType imageType() const noexcept;
    inline VkFormat format() const noexcept;
    /// Extent of base level
    inline const glm::uvec3& extent() const noexcept;
    inline glm::uvec3 extent(uint32_t mipLevel) const noexcept;
    inline VkSampleCountFlagBits samples() const noexcept;
    inline uint32_t arraySize() const noexcept;
    inline uint32_t mipmapCount() const noexcept;
    inline size_t memorySize() const noexcept;
    inline VkImageLayout primaryLayout() const noexcept;
    /// Should be called one time after constructor
    void scheduleInitLayout(VkImageAspectFlags aspectMask,
                            CommandProducer& producer);

    /// Move data from GPU to buffer
    void readData(VkImageAspectFlags srcAspect,
                  uint32_t srcArrayIndex,
                  uint32_t srcMipLevel,
                  glm::uvec3 srcOffset,
                  glm::uvec3 srcExtent,
                  void* dstBuffer,
                  size_t dataSize,
                  VkDeviceSize dstBufferOffset,
                  uint32_t dstRowLength,
                  uint32_t dstImageHeight);

    struct CopyFromBufferChunk
    {
      VkDeviceSize srcBufferOffset;
      uint32_t srcRowLength;
      uint32_t srcImageHeight;

      uint32_t dstArrayIndex;
      uint32_t dstMipLevel;
      glm::uvec3 dstOffset;
      glm::uvec3 dstExtent;
    };
    void scheduleCopy(CommandProducer& producer,
                      PlainBuffer& srcBuffer,
                      VkImageAspectFlags dstAspect,
                      const std::vector<CopyFromBufferChunk>& copyInfo);

    struct CopyToBufferChunk
    {
      uint32_t srcArrayIndex;
      uint32_t srcMipLevel;
      glm::uvec3 srcOffset;
      glm::uvec3 srcExtent;

      VkDeviceSize dstBufferOffset;
      uint32_t dstRowLength;
      uint32_t dstImageHeight;
    };
    void scheduleCopy(CommandProducer& producer,
                      PlainBuffer& dstBuffer,
                      VkImageAspectFlags srcAspect,
                      const std::vector<CopyToBufferChunk>& copyInfo);

    struct CopyImageChunk
    {
      uint32_t srcArrayIndex;
      uint32_t srcMipLevel;
      glm::uvec3 srcOffset;

      uint32_t dstArrayIndex;
      uint32_t dstMipLevel;
      glm::uvec3 dstOffset;

      glm::uvec3 extent;
    };
    void scheduleCopy(CommandProducer& producer,
                      Image& srcImage,
                      VkImageAspectFlags srcAspect,
                      VkImageAspectFlags dstAspect,
                      const std::vector<CopyImageChunk>& copyInfo);

    struct BlitChunk
    {
      uint32_t srcArrayIndex;
      uint32_t srcMipLevel;
      glm::uvec3 srcCorners[2];

      uint32_t dstArrayIndex;
      uint32_t dstMipLevel;
      glm::uvec3 dstCorners[2];
    };
    void scheduleBlit(CommandProducer& producer,
                      Image& srcImage,
                      VkImageAspectFlags srcAspect,
                      VkImageAspectFlags dstAspect,
                      const std::vector<BlitChunk>& blitInfo,
                      VkFilter filter);

    struct ClearChunk
    {
      uint32_t arrayIndex;
      uint32_t mipLevel;
    };
    /// Only VK_IMAGE_ASPECT_COLOR_BIT and VK_IMAGE_ASPECT_DEPTH_BIT
    /// are supported
    void scheduleClear( CommandProducer& producer,
                        VkImageAspectFlags aspect,
                        const VkClearValue& clearValue,
                        const std::vector<ClearChunk>& clearChunks);

    /// Warning!!! You should return image to primary layout after using
    void scheduleChangePrimaryLayout( VkImageAspectFlags aspectMask,
                                      VkImageLayout newLayout,
                                      CommandProducer& producer);

    void scheduleMipGeneration( uint32_t baseArrayLayer,
                                uint32_t layerCount,
                                VkImageAspectFlags aspectMask,
                                CommandProducer& producer);

    inline static uint32_t calculateMipNumber(const glm::uvec2& extent);

  private:
    void _buildEmpty( VkImageUsageFlags usageFlags,
                      VkImageCreateFlags createFlags,
                      bool initLayout,
                      VkImageAspectFlags initAspectMask);
    void _cleanup() noexcept;
    void _scheduleImageBarrier( CommandProducer& producer,
                                VkImageLayout oldLayout,
                                VkImageLayout newLayout,
                                VkImageAspectFlags aspectMask,
                                uint32_t baseMip,
                                uint32_t mipCount,
                                uint32_t baseArrayLayer,
                                uint32_t layerCount);

  private:
    VkImage _handle;

    VmaAllocation _allocation;
    size_t _memorySize;

    VkImageType _imageType;
    VkFormat _format;

    glm::uvec3 _extent;
    VkSampleCountFlagBits _samples;
    uint32_t _arraySize;
    uint32_t _mipmapCount;

    VkImageLayout _primaryLayout;
    LogicalDevice& _device;
  };

  inline LogicalDevice& Image::device() noexcept
  {
    return _device;
  }

  inline const LogicalDevice& Image::device() const noexcept
  {
    return _device;
  }

  inline VkImage Image::handle() const noexcept
  {
    return _handle;
  }

  inline VkImageType Image::imageType() const noexcept
  {
    return _imageType;
  }

  inline VkFormat Image::format() const noexcept
  {
    return _format;
  }

  inline const glm::uvec3& Image::extent() const noexcept
  {
    return _extent;
  }

  inline glm::uvec3 Image::extent(uint32_t mipLevel) const noexcept
  {
    return glm::max(glm::uvec3( _extent.x >> mipLevel,
                                _extent.y >> mipLevel,
                                _extent.z >> mipLevel),
                    glm::uvec3(1));
  }

  inline VkSampleCountFlagBits Image::samples() const noexcept
  {
    return _samples;
  }

  inline uint32_t Image::arraySize() const noexcept
  {
    return _arraySize;
  }

  inline uint32_t Image::mipmapCount() const noexcept
  {
    return _mipmapCount;
  }

  inline VkImageLayout Image::primaryLayout() const noexcept
  {
    return _primaryLayout;
  }

  inline size_t Image::memorySize() const noexcept
  {
    return _memorySize;
  }

  inline uint32_t Image::calculateMipNumber(const glm::uvec2& extent)
  {
    unsigned int maxDimension = std::max(extent.x, extent.y);
    return uint32_t(std::floor(std::log2(maxDimension)) + 1);
  }
}