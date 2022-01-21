#include <glm/glm.hpp>

#include <mtt/render/CommandQueue/CommandProducer.h>
#include <mtt/render/Image.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/render/PlainBuffer.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

Image::Image( VkImageType imageType,
              VkImageLayout primaryLayout,
              VkImageUsageFlags usageFlags,
              VkImageCreateFlags createFlags,
              VkFormat format,
              glm::uvec3 extent,
              VkSampleCountFlagBits samples,
              uint32_t arraySize,
              uint32_t mipmapCount,
              VkImageAspectFlags initAspectMask,
              LogicalDevice& device) :
  _handle(VK_NULL_HANDLE),
  _allocation(VK_NULL_HANDLE),
  _memorySize(0),
  _imageType(imageType),
  _format(format),
  _extent(extent),
  _samples(samples),
  _arraySize(arraySize),
  _mipmapCount(mipmapCount),
  _primaryLayout(primaryLayout),
  _device(device)
{
  _buildEmpty(usageFlags, createFlags, true, initAspectMask);
}

Image::Image( VkImageType imageType,
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
              LogicalDevice& device) :
  _handle(VK_NULL_HANDLE),
  _allocation(VK_NULL_HANDLE),
  _imageType(imageType),
  _format(format),
  _extent(extent),
  _samples(samples),
  _arraySize(1),
  _mipmapCount(mipmapCount),
  _primaryLayout(primaryLayout),
  _device(device)
{
  if(data == nullptr) Abort("Image::Image: data is nullptr.");
  if(datasize == 0) Abort("Image::Image: datasize == 0.");

  _buildEmpty(usageFlags | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
              createFlags,
              false,
              dstAspect);

  try
  {
    Ref<PlainBuffer> uploadBuffer;
    uploadBuffer = new PlainBuffer( _device,
                                    datasize,
                                    PlainBuffer::UPLOAD_BUFFER);
    uploadBuffer->uploadData(data, 0, datasize);

    auto transferCommand =
      [&](CommandProducer& uploadProducer)
      {
        scheduleInitLayout(dstAspect, uploadProducer);

        CopyFromBufferChunk copyInfo;
        copyInfo.srcBufferOffset = 0;
        copyInfo.srcRowLength = srcRowLength;
        copyInfo.srcImageHeight = srcImageHeight;
        copyInfo.dstArrayIndex = 0;
        copyInfo.dstMipLevel = 0;
        copyInfo.dstOffset = glm::vec3(0);
        copyInfo.dstExtent = _extent;
        std::vector<Image::CopyFromBufferChunk> infoVec{copyInfo};
        scheduleCopy(uploadProducer, *uploadBuffer, dstAspect, infoVec);

        if(_mipmapCount > 1)
        {
          scheduleMipGeneration(0,
                                _arraySize,
                                dstAspect,
                                uploadProducer);
        }
      };
    _device.runTransferCommand(transferCommand);
  }
  catch(...)
  {
    _cleanup();
    throw;
  }
}

void Image::_buildEmpty(VkImageUsageFlags usageFlags,
                        VkImageCreateFlags createFlags,
                        bool initLayout,
                        VkImageAspectFlags initAspectMask)
{
  try
  {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = _imageType;
    imageInfo.extent.width = uint32_t(_extent.x);
    imageInfo.extent.height = uint32_t(_extent.y);
    imageInfo.extent.depth = uint32_t(_extent.z);;
    imageInfo.mipLevels = _mipmapCount;
    imageInfo.arrayLayers = _arraySize;
    imageInfo.format = _format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usageFlags;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = _samples;
    imageInfo.flags = createFlags;

    VmaAllocationCreateInfo allocCreateInfo{};
    allocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    VmaAllocationInfo allocInfo{};

    if(vmaCreateImage(_device.allocator(),
                      &imageInfo,
                      &allocCreateInfo,
                      &_handle,
                      &_allocation,
                      &allocInfo) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to create image.");
    }

    _memorySize = allocInfo.size;

    if(initLayout)
    {
      auto initCommand =
        [&](CommandProducer& uploadProducer)
        {
          scheduleInitLayout(initAspectMask, uploadProducer);
        };
      _device.runTransferCommand(initCommand);
    }
  }
  catch(...)
  {
    _cleanup();
    throw;
  }
}

Image::Image( VkImage handle,
              VkImageType imageType,
              VkImageLayout primaryLayout,
              VkFormat format,
              glm::uvec3 extent,
              VkSampleCountFlagBits samples,
              uint32_t arraySize,
              uint32_t mipmapCount,
              LogicalDevice& device) :
  _handle(handle),
  _allocation(VK_NULL_HANDLE),
  _imageType(imageType),
  _format(format),
  _extent(extent),
  _samples(samples),
  _arraySize(arraySize),
  _mipmapCount(mipmapCount),
  _primaryLayout(primaryLayout),
  _device(device)
{
  VkMemoryRequirements memoryRequirements;
  vkGetImageMemoryRequirements( _device.handle(),
                                _handle,
                                &memoryRequirements);
  _memorySize = memoryRequirements.size;
}

Image::~Image()
{
  _cleanup();
}

void Image::_cleanup() noexcept
{
  if(_handle != VK_NULL_HANDLE && _allocation != VK_NULL_HANDLE)
  {
    vmaDestroyImage(_device.allocator(), _handle, _allocation);
    _handle = VK_NULL_HANDLE;
    _allocation = VK_NULL_HANDLE;
  }
}

void Image::_scheduleImageBarrier(CommandProducer& producer,
                                  VkImageLayout oldLayout,
                                  VkImageLayout newLayout,
                                  VkImageAspectFlags aspectMask,
                                  uint32_t baseMip,
                                  uint32_t mipCount,
                                  uint32_t baseArrayLayer,
                                  uint32_t layerCount)
{
  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = _handle;
  barrier.subresourceRange.aspectMask = aspectMask;
  barrier.subresourceRange.baseMipLevel = baseMip;
  barrier.subresourceRange.levelCount = mipCount;
  barrier.subresourceRange.baseArrayLayer = baseArrayLayer;
  barrier.subresourceRange.layerCount = layerCount;

  barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT |
                          VK_ACCESS_MEMORY_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT |
                          VK_ACCESS_MEMORY_WRITE_BIT;

  vkCmdPipelineBarrier( producer.bufferHandle(),
                        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                        0,
                        0,
                        nullptr,
                        0,
                        nullptr,
                        1,
                        &barrier);
}

void Image::scheduleInitLayout( VkImageAspectFlags aspectMask,
                                CommandProducer& producer)
{
  _scheduleImageBarrier(producer,
                        VK_IMAGE_LAYOUT_UNDEFINED,
                        _primaryLayout,
                        aspectMask,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);
  producer.lockResource(*this);
}

void Image::readData( VkImageAspectFlags srcAspect,
                      uint32_t srcArrayIndex,
                      uint32_t srcMipLevel,
                      glm::uvec3 srcOffset,
                      glm::uvec3 srcExtent,
                      void* dstBuffer,
                      size_t dataSize,
                      VkDeviceSize dstBufferOffset,
                      uint32_t dstRowLength,
                      uint32_t dstImageHeight)
{
  Ref<PlainBuffer> downloadBuffer(
                                new PlainBuffer(_device,
                                                dataSize,
                                                PlainBuffer::DOWNLOAD_BUFFER));

  auto transferCommand =
    [&](CommandProducer& uploadProducer)
    {
      Image::CopyToBufferChunk copyInfo;
      copyInfo.srcArrayIndex = srcArrayIndex;
      copyInfo.srcMipLevel = srcMipLevel;
      copyInfo.srcOffset = srcOffset;
      copyInfo.srcExtent = srcExtent;
      copyInfo.dstBufferOffset = dstBufferOffset;
      copyInfo.dstRowLength = dstRowLength;
      copyInfo.dstImageHeight = dstImageHeight;
      std::vector<Image::CopyToBufferChunk> infoVec{copyInfo};
      scheduleCopy(uploadProducer, *downloadBuffer, srcAspect, infoVec);
    };
  _device.runTransferCommand(transferCommand);

  PlainBuffer::MemoryMapper mapper(*downloadBuffer);
  memcpy(dstBuffer, mapper.data(), dataSize);
}

void Image::scheduleCopy( CommandProducer& producer,
                          PlainBuffer& srcBuffer,
                          VkImageAspectFlags dstAspect,
                          const std::vector<CopyFromBufferChunk>& copyInfo)
{
  _scheduleImageBarrier( producer,
                        _primaryLayout,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        dstAspect,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);

  for(const CopyFromBufferChunk& chunk : copyInfo)
  {
    VkBufferImageCopy region{};
    region.bufferOffset = chunk.srcBufferOffset;
    region.bufferRowLength = chunk.srcRowLength;
    region.bufferImageHeight = chunk.srcImageHeight;

    region.imageSubresource.aspectMask = dstAspect;
    region.imageSubresource.mipLevel = chunk.dstMipLevel;
    region.imageSubresource.baseArrayLayer = chunk.dstArrayIndex;
    region.imageSubresource.layerCount = 1;

    region.imageOffset.x = uint32_t(chunk.dstOffset.x);
    region.imageOffset.y = uint32_t(chunk.dstOffset.y);
    region.imageOffset.z = uint32_t(chunk.dstOffset.z);

    region.imageExtent.width = uint32_t(chunk.dstExtent.x);
    region.imageExtent.height = uint32_t(chunk.dstExtent.y);
    region.imageExtent.depth = uint32_t(chunk.dstExtent.z);

    vkCmdCopyBufferToImage( producer.bufferHandle(),
                            srcBuffer.handle(),
                            _handle,
                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                            1,
                            &region);
  }

  _scheduleImageBarrier(producer,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        _primaryLayout,
                        dstAspect,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);

  producer.lockResource(srcBuffer);
  producer.lockResource(*this);
}

void Image::scheduleCopy( CommandProducer& producer,
                          PlainBuffer& dstBuffer,
                          VkImageAspectFlags srcAspect,
                          const std::vector<CopyToBufferChunk>& copyInfo)
{
  _scheduleImageBarrier( producer,
                        _primaryLayout,
                        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                        srcAspect,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);

  for(const CopyToBufferChunk& chunk : copyInfo)
  {
    VkBufferImageCopy region{};

    region.imageSubresource.aspectMask = srcAspect;
    region.imageSubresource.mipLevel = chunk.srcMipLevel;
    region.imageSubresource.baseArrayLayer = chunk.srcArrayIndex;
    region.imageSubresource.layerCount = 1;

    region.imageOffset.x = uint32_t(chunk.srcOffset.x);
    region.imageOffset.y = uint32_t(chunk.srcOffset.y);
    region.imageOffset.z = uint32_t(chunk.srcOffset.z);

    region.imageExtent.width = uint32_t(chunk.srcExtent.x);
    region.imageExtent.height = uint32_t(chunk.srcExtent.y);
    region.imageExtent.depth = uint32_t(chunk.srcExtent.z);

    region.bufferOffset = chunk.dstBufferOffset;
    region.bufferRowLength = chunk.dstRowLength;
    region.bufferImageHeight = chunk.dstImageHeight;

    vkCmdCopyImageToBuffer( producer.bufferHandle(),
                            _handle,
                            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                            dstBuffer.handle(),
                            1,
                            &region);
  }

  _scheduleImageBarrier(producer,
                        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                        _primaryLayout,
                        srcAspect,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);

  producer.lockResource(dstBuffer);
  producer.lockResource(*this);
}

void Image::scheduleCopy( CommandProducer& producer,
                          Image& srcImage,
                          VkImageAspectFlags srcAspect,
                          VkImageAspectFlags dstAspect,
                          const std::vector<CopyImageChunk>& copyInfo)
{
  srcImage._scheduleImageBarrier(producer,
                                srcImage._primaryLayout,
                                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                srcAspect,
                                0,
                                srcImage._mipmapCount,
                                0,
                                srcImage._arraySize);
  _scheduleImageBarrier( producer,
                        _primaryLayout,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        dstAspect,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);

  for(const CopyImageChunk& chunk : copyInfo)
  {
    VkImageCopy region{};
    region.srcSubresource.aspectMask = srcAspect;
    region.srcSubresource.mipLevel = chunk.srcMipLevel;
    region.srcSubresource.baseArrayLayer = chunk.srcArrayIndex;
    region.srcSubresource.layerCount = 1;
    region.srcOffset.x = uint32_t(chunk.srcOffset.x);
    region.srcOffset.y = uint32_t(chunk.srcOffset.y);
    region.srcOffset.z = uint32_t(chunk.srcOffset.z);

    region.dstSubresource.aspectMask = dstAspect;
    region.dstSubresource.mipLevel = chunk.dstMipLevel;
    region.dstSubresource.baseArrayLayer = chunk.dstArrayIndex;
    region.dstSubresource.layerCount = 1;
    region.dstOffset.x = uint32_t(chunk.dstOffset.x);
    region.dstOffset.y = uint32_t(chunk.dstOffset.y);
    region.dstOffset.z = uint32_t(chunk.dstOffset.z);

    region.extent.width = uint32_t(chunk.extent.x);
    region.extent.height = uint32_t(chunk.extent.y);
    region.extent.depth = uint32_t(chunk.extent.z);

    vkCmdCopyImage( producer.bufferHandle(),
                    srcImage.handle(),
                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    _handle,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    1,
                    &region);
  }

  _scheduleImageBarrier(producer,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        _primaryLayout,
                        dstAspect,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);
  srcImage._scheduleImageBarrier( producer,
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  srcImage._primaryLayout,
                                  srcAspect,
                                  0,
                                  srcImage._mipmapCount,
                                  0,
                                  srcImage._arraySize);

  producer.lockResource(srcImage);
  producer.lockResource(*this);
}

void Image::scheduleBlit( CommandProducer& producer,
                          Image& srcImage,
                          VkImageAspectFlags srcAspect,
                          VkImageAspectFlags dstAspect,
                          const std::vector<BlitChunk>& blitInfo,
                          VkFilter filter)
{
  srcImage._scheduleImageBarrier( producer,
                                  srcImage._primaryLayout,
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  srcAspect,
                                  0,
                                  srcImage._mipmapCount,
                                  0,
                                  srcImage._arraySize);
  _scheduleImageBarrier(producer,
                        _primaryLayout,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        dstAspect,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);

  for(const BlitChunk& chunk : blitInfo)
  {
    VkImageBlit region{};
    region.srcSubresource.aspectMask = srcAspect;
    region.srcSubresource.mipLevel = chunk.srcMipLevel;
    region.srcSubresource.baseArrayLayer = chunk.srcArrayIndex;
    region.srcSubresource.layerCount = 1;
    region.srcOffsets[0].x = uint32_t(chunk.srcCorners[0].x);
    region.srcOffsets[0].y = uint32_t(chunk.srcCorners[0].y);
    region.srcOffsets[0].z = uint32_t(chunk.srcCorners[0].z);
    region.srcOffsets[1].x = uint32_t(chunk.srcCorners[1].x);
    region.srcOffsets[1].y = uint32_t(chunk.srcCorners[1].y);
    region.srcOffsets[1].z = uint32_t(chunk.srcCorners[1].z);

    region.dstSubresource.aspectMask = dstAspect;
    region.dstSubresource.mipLevel = chunk.dstMipLevel;
    region.dstSubresource.baseArrayLayer = chunk.dstArrayIndex;
    region.dstSubresource.layerCount = 1;
    region.dstOffsets[0].x = uint32_t(chunk.dstCorners[0].x);
    region.dstOffsets[0].y = uint32_t(chunk.dstCorners[0].y);
    region.dstOffsets[0].z = uint32_t(chunk.dstCorners[0].z);
    region.dstOffsets[1].x = uint32_t(chunk.dstCorners[1].x);
    region.dstOffsets[1].y = uint32_t(chunk.dstCorners[1].y);
    region.dstOffsets[1].z = uint32_t(chunk.dstCorners[1].z);

    vkCmdBlitImage( producer.bufferHandle(),
                    srcImage.handle(),
                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    _handle,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    1,
                    &region,
                    filter);
  }

  _scheduleImageBarrier(producer,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        _primaryLayout,
                        dstAspect,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);
  srcImage._scheduleImageBarrier( producer,
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  srcImage._primaryLayout,
                                  srcAspect,
                                  0,
                                  srcImage._mipmapCount,
                                  0,
                                  srcImage._arraySize);

  producer.lockResource(srcImage);
  producer.lockResource(*this);
}

void Image::scheduleClear(CommandProducer& producer,
                          VkImageAspectFlags aspect,
                          const VkClearValue& clearValue,
                          const std::vector<ClearChunk>& clearChunks)
{
  _scheduleImageBarrier(producer,
                        _primaryLayout,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        aspect,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);

  for(const ClearChunk& chunk : clearChunks)
  {
    VkImageSubresourceRange range{};
    range.aspectMask = aspect;
    range.baseMipLevel = chunk.mipLevel;
    range.levelCount = 1;
    range.baseArrayLayer = chunk.arrayIndex;
    range.layerCount = 1;

    switch(aspect)
    {
    case VK_IMAGE_ASPECT_COLOR_BIT :
      vkCmdClearColorImage( producer.bufferHandle(),
                            handle(),
                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                            &clearValue.color,
                            1,
                            &range);
      break;
    case VK_IMAGE_ASPECT_DEPTH_BIT:
      vkCmdClearDepthStencilImage(producer.bufferHandle(),
                                  handle(),
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  &clearValue.depthStencil,
                                  1,
                                  &range);
    }
  }

  _scheduleImageBarrier(producer,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        _primaryLayout,
                        aspect,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);

  producer.lockResource(*this);
}

void Image::scheduleChangePrimaryLayout(VkImageAspectFlags aspectMask,
                                        VkImageLayout newLayout,
                                        CommandProducer& producer)
{
  if(_primaryLayout == newLayout) return;
  _scheduleImageBarrier(producer,
                        _primaryLayout,
                        newLayout,
                        aspectMask,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);
  _primaryLayout = newLayout;
}

void Image::scheduleMipGeneration(uint32_t baseArrayLayer,
                                  uint32_t layerCount,
                                  VkImageAspectFlags aspectMask,
                                  CommandProducer& producer)
{
  if(_mipmapCount == 1) return;

  _scheduleImageBarrier(producer,
                        _primaryLayout,
                        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                        aspectMask,
                        0,
                        1,
                        0,
                        _arraySize);
  _scheduleImageBarrier(producer,
                        _primaryLayout,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        aspectMask,
                        1,
                        _mipmapCount - 1,
                        0,
                        _arraySize);

  glm::ivec3 baseExtent = _extent;
  for(uint32_t mipLevel = 1; mipLevel < _mipmapCount; mipLevel++)
  {
    glm::ivec3 mipExtent( baseExtent.x / 2,
                          baseExtent.y / 2,
                          baseExtent.z / 2);
    mipExtent = glm::max(mipExtent, glm::ivec3(1));
  
    VkImageBlit blitInfo{};
    blitInfo.srcOffsets[0] = { 0, 0, 0 };
    blitInfo.srcOffsets[1] = { baseExtent.x, baseExtent.y, baseExtent.z };
    blitInfo.srcSubresource.aspectMask = aspectMask;
    blitInfo.srcSubresource.mipLevel = mipLevel - 1;
    blitInfo.srcSubresource.baseArrayLayer = baseArrayLayer;
    blitInfo.srcSubresource.layerCount = layerCount;
    blitInfo.dstOffsets[0] = { 0, 0, 0 };
    blitInfo.dstOffsets[1] = { mipExtent.x, mipExtent.y, mipExtent.z };
    blitInfo.dstSubresource.aspectMask = aspectMask;
    blitInfo.dstSubresource.mipLevel = mipLevel;
    blitInfo.dstSubresource.baseArrayLayer = baseArrayLayer;
    blitInfo.dstSubresource.layerCount = layerCount;

    vkCmdBlitImage( producer.bufferHandle(),
                    _handle,
                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    _handle,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    1,
                    &blitInfo,
                    VK_FILTER_LINEAR);

    _scheduleImageBarrier(producer,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                          VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                          aspectMask,
                          mipLevel,
                          1,
                          0,
                          _arraySize);

    baseExtent = mipExtent;
  }

  _scheduleImageBarrier(producer,
                        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                        _primaryLayout,
                        aspectMask,
                        0,
                        _mipmapCount,
                        0,
                        _arraySize);
}
