#include <mtt/Render/Pipeline/ComputePipeline.h>
#include <mtt/Render/Pipeline/CubeTexture.h>
#include <mtt/Render/Pipeline/Sampler.h>
#include <mtt/Render/Pipeline/StorageImageResource.h>
#include <mtt/Render/LogicalDevice.h>
#include <mtt/Render/PlainBuffer.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

CubeTexture::CubeTexture(LogicalDevice& device) noexcept :
  AbstractTexture(CUBE_TEXTURE, device),
  _dataFormat(VK_FORMAT_R8G8B8A8_SRGB),
  _sideExtent(1),
  _samples(VK_SAMPLE_COUNT_1_BIT),
  _lodCount(1)
{
  Ref<Image> newImage = _buildImage(_dataFormat,
                                    _sideExtent,
                                    _samples,
                                    _lodCount,
                                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                    0);
  setView(_buildImageView(*newImage).get());
}

Ref<Image> CubeTexture::_buildImage(VkFormat dataFormat,
                                    uint32_t sideExtent,
                                    VkSampleCountFlagBits samples,
                                    uint32_t lodCount,
                                    VkImageLayout layout,
                                    VkImageUsageFlags usageFlags)
{
  return Ref<Image>(new Image(VK_IMAGE_TYPE_2D,
                              layout,
                              usageFlags |
                                VK_IMAGE_USAGE_SAMPLED_BIT,
                              VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
                              dataFormat,
                              glm::uvec3( sideExtent,
                                          sideExtent,
                                          1),
                              samples,
                              6,
                              lodCount,
                              VK_IMAGE_ASPECT_COLOR_BIT,
                              device()));
}

Ref<ImageView> CubeTexture::_buildImageView(Image& image)
{
  VkComponentMapping colorMapping;
  colorMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  VkImageSubresourceRange subresourceRange;
  subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  subresourceRange.baseMipLevel = 0;
  subresourceRange.levelCount = image.mipmapCount();
  subresourceRange.baseArrayLayer = 0;
  subresourceRange.layerCount = 6;

  Ref<ImageView> imageView(new ImageView( image,
                                          VK_IMAGE_VIEW_TYPE_CUBE,
                                          colorMapping,
                                          subresourceRange));
  return imageView;
}

void CubeTexture::setData(VkFormat dataFormat,
                          uint32_t extent,
                          VkSampleCountFlagBits samples,
                          const SideDataInfo dataInfo[6],
                          bool generateLods)
{
  if(extent == 0) Abort("CubeTexture::setData: extent is null");

  uint32_t newLodCount = 1;
  if(generateLods) newLodCount = Image::calculateMipNumber(glm::uvec2(extent));

  Ref<Image> newImage = _buildImage(dataFormat,
                                    extent,
                                    samples,
                                    newLodCount,
                                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                    VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT);

  for(uint32_t side = 0; side < 6; side++)
  {
    _setSideData(*newImage, side, dataInfo[side], extent);
  }

  Ref<ImageView> newView = _buildImageView(*newImage);

  setView(newView.get());

  _dataFormat = dataFormat;
  _sideExtent = extent;
  _samples = samples;
  _lodCount = newLodCount;
}

void CubeTexture::_setSideData( Image& image,
                                uint32_t side,
                                SideDataInfo dataInfo,
                                uint32_t sideExtent)
{
  if(dataInfo.data == nullptr) return;

  if(dataInfo.datasize == 0) Abort("CubeTexture::setSideData: datasize is null");
  if(dataInfo.srcRowLength == 0) Abort("CubeTexture::setSideData: srcRowLength is null");

  Ref<PlainBuffer> uploadBuffer(new PlainBuffer(device(),
                                                dataInfo.datasize,
                                                PlainBuffer::UPLOAD_BUFFER));
  uploadBuffer->uploadData(dataInfo.data, 0, dataInfo.datasize);

  Image::CopyFromBufferChunk copyInfo{};
  copyInfo.srcBufferOffset = 0;
  copyInfo.srcRowLength = dataInfo.srcRowLength;
  copyInfo.srcImageHeight = sideExtent;
  copyInfo.dstArrayIndex = side;
  copyInfo.dstMipLevel = 0;
  copyInfo.dstOffset = glm::uvec3(0);
  copyInfo.dstExtent = glm::uvec3(sideExtent, sideExtent, 1);

  auto transferCommand =
    [&](CommandProducer& uploadProducer)
    {
      image.scheduleCopy( uploadProducer,
                          *uploadBuffer,
                          VK_IMAGE_ASPECT_COLOR_BIT,
                          {copyInfo});
      image.scheduleMipGeneration(side,
                                  1,
                                  VK_IMAGE_ASPECT_COLOR_BIT,
                                  uploadProducer);
    };
  device().runTransferCommand(transferCommand);
}

void CubeTexture::buildDiffuseLuminanceMap(
                                std::shared_ptr<CubeTexture> environmentTexture,
                                uint32_t extent)
{
  Ref<Image> newImage = _buildImage(VK_FORMAT_R8G8B8A8_UNORM,
                                    extent,
                                    VK_SAMPLE_COUNT_1_BIT,
                                    1,
                                    VK_IMAGE_LAYOUT_GENERAL,
                                    VK_IMAGE_USAGE_STORAGE_BIT);

  VkComponentMapping colorMapping;
  colorMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  VkImageSubresourceRange subresourceRange;
  subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  subresourceRange.baseMipLevel = 0;
  subresourceRange.levelCount = 1;
  subresourceRange.baseArrayLayer = 0;
  subresourceRange.layerCount = 6;

  Ref<ImageView> writeView(new ImageView( *newImage,
                                          VK_IMAGE_VIEW_TYPE_2D_ARRAY,
                                          colorMapping,
                                          subresourceRange));

  StorageImageResource imageStorage(PipelineResource::STATIC);
  imageStorage.setAttachedImageView(writeView.get());
  
  Sampler sampler(PipelineResource::STATIC, device());
  sampler.setAttachedTexture(environmentTexture, 0);

  ComputePipeline pipeline(device());
  std::unique_ptr<ShaderModule> shader(
                                new ShaderModule( ShaderModule::COMPUTE_SHADER,
                                                  device()));
  shader->newFragment().loadFromFile("buildDiffuseLuminanceMap.comp");
  pipeline.setShader(std::move(shader));

  pipeline.addResource( "luminanceTextureBinding",
                        sampler,
                        VK_SHADER_STAGE_COMPUTE_BIT);
  
  pipeline.addResource( "storageTextureBinding",
                        imageStorage,
                        VK_SHADER_STAGE_COMPUTE_BIT);

  uint32_t xSize = extent / 8;
  if (extent % 8 != 0) xSize++;

  uint32_t ySize = extent / 4;
  if (extent % 4 != 0) ySize++;

  auto buildCommand =
    [&](CommandProducer& producer)
    {
      pipeline.scheduleBind(producer);
      vkCmdDispatch(producer.bufferHandle(), xSize, ySize, 6);

      newImage->scheduleChangePrimaryLayout(
                                      VK_IMAGE_ASPECT_COLOR_BIT,
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                      producer);
    };
  device().runTransferCommand(buildCommand);

  setView(_buildImageView(*newImage).get());
  _dataFormat = VK_FORMAT_R8G8B8A8_SRGB;
  _sideExtent = extent;
  _samples = VK_SAMPLE_COUNT_1_BIT;
  _lodCount = 1;
}
