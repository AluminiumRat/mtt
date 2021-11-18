#include <stdexcept>

#include <mtt/Render/Pipeline/Texture2D.h>
#include <mtt/Render/LogicalDevice.h>
#include <mtt/Render/PlainBuffer.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

Texture2D::Texture2D(LogicalDevice& device)  noexcept :
  AbstractTexture(TEXTURE_2D, device),
  _dataFormat(VK_FORMAT_R8G8B8A8_SRGB),
  _extent(1, 1),
  _samples(VK_SAMPLE_COUNT_1_BIT),
  _lodCount(1)
{
  makeEmpty(_dataFormat, _extent, _samples, _lodCount);
}

void Texture2D::makeEmpty(VkFormat dataFormat,
                          const glm::uvec2& extent,
                          VkSampleCountFlagBits samples,
                          uint32_t lodCount)
{
  if(extent.x == 0 || extent.y == 0) Abort("Texture2D::makeEmpty: invalid extent.");
  if(lodCount == 0) Abort("Texture2D::makeEmpty: lodCount is 0.");

  Ref<Image> newImage = _buildImage(nullptr,
                                    0,
                                    0,
                                    dataFormat,
                                    extent,
                                    samples,
                                    lodCount);

  Ref<ImageView> newImageView = _buildImageView(*newImage,
                                                VK_IMAGE_ASPECT_COLOR_BIT);
  setView(newImageView.get());

  _dataFormat = dataFormat;
  _extent = extent;
  _samples = samples;
  _lodCount = lodCount;
}

void Texture2D::setData(const void* data,
                        size_t datasize,
                        VkFormat dataFormat,
                        const glm::uvec2& extent,
                        VkSampleCountFlagBits samples,
                        uint32_t srcRowLength,
                        bool generateLods)
{
  if (data == nullptr) Abort("Texture2D::setData: data is null.");
  if (datasize == 0) Abort("Texture2D::setData: datasize is null.");
  if (extent.x == 0 || extent.y == 0) Abort("Texture2D::setData: invalid extent.");

  uint32_t newLodCount;
  if (generateLods) newLodCount = Image::calculateMipNumber(extent);
  else newLodCount = 1;

  Ref<Image> newImage = _buildImage(data,
                                    datasize,
                                    srcRowLength,
                                    dataFormat,
                                    extent,
                                    samples,
                                    newLodCount);
  Ref<ImageView> newImageView = _buildImageView(*newImage,
                                                VK_IMAGE_ASPECT_COLOR_BIT);

  setView(newImageView.get());

  _dataFormat = dataFormat;
  _extent = extent;
  _samples = samples;
  _lodCount = newLodCount;
}

Ref<Image> Texture2D::_buildImage(const void* data,
                                  size_t datasize,
                                  uint32_t srcRowLength,
                                  VkFormat dataFormat,
                                  const glm::uvec2& extent,
                                  VkSampleCountFlagBits samples,
                                  uint32_t lodCount)
{
  if(datasize != 0)
  {
    Ref<Image> newImage(new Image(VK_IMAGE_TYPE_2D,
                                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                  VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                                VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                                VK_IMAGE_USAGE_SAMPLED_BIT,
                                  0,
                                  dataFormat,
                                  glm::uvec3(extent, 1),
                                  samples,
                                  lodCount,
                                  VK_IMAGE_ASPECT_COLOR_BIT,
                                  data,
                                  datasize,
                                  srcRowLength,
                                  0,
                                  device()));
    return newImage;
  }
  else
  {
    Ref<Image> newImage(new Image(VK_IMAGE_TYPE_2D,
                                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                  VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                                VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                                VK_IMAGE_USAGE_SAMPLED_BIT,
                                  0,
                                  dataFormat,
                                  glm::uvec3(extent, 1),
                                  samples,
                                  1,
                                  lodCount,
                                  VK_IMAGE_ASPECT_COLOR_BIT,
                                  device()));
    return newImage;
  }
}

void Texture2D::setImage(Image& newImage, VkImageAspectFlags dataAspect)
{
  if(newImage.imageType() != VK_IMAGE_TYPE_2D) Abort("Texture2D::setImage: wrong image type.");

  Ref<ImageView> newImageView = _buildImageView(newImage, dataAspect);

  setView(newImageView.get());

  _dataFormat = newImage.format();
  _extent = newImage.extent();
  _samples = newImage.samples();
  _lodCount = newImage.mipmapCount();
}

Ref<ImageView> Texture2D::_buildImageView(Image& image,
                                          VkImageAspectFlags dataAspect)
{
  VkComponentMapping colorMapping;
  colorMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  VkImageSubresourceRange subresourceRange;
  subresourceRange.aspectMask = dataAspect;
  subresourceRange.baseMipLevel = 0;
  subresourceRange.levelCount = image.mipmapCount();
  subresourceRange.baseArrayLayer = 0;
  subresourceRange.layerCount = 1;

  Ref<ImageView> imageView(new ImageView( image,
                                          VK_IMAGE_VIEW_TYPE_2D,
                                          colorMapping,
                                          subresourceRange));
  return imageView;
}

void Texture2D::setImageView(ImageView& newImageView)
{
  if(newImageView.viewType() != VK_IMAGE_VIEW_TYPE_2D)
  {
    Abort("Texture2D::setImageView: wrong imageview type.");
  }

  setView(&newImageView);

  _extent = newImageView.extent();
  _dataFormat = newImageView.image().format();
  _samples = newImageView.image().samples();
  _lodCount = newImageView.subresourceRange().levelCount;
}
