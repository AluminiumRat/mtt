#include <stdexcept>

#include <mtt/Core/ResourceManager/CubeTextureLibrary.h>
#include <mtt/Render/Pipeline/CubeTexture.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

std::shared_ptr<CubeTexture> CubeTextureLibrary::load(
                                              std::array<QString, 6> filenames,
                                              LogicalDevice& device,
                                              bool generateLods)
{
  CubeTextureDescription description{ filenames,
                                      &device,
                                      VK_SAMPLE_COUNT_1_BIT,
                                      generateLods};
  return getOrCreate(description);
}

std::shared_ptr<CubeTexture> CubeTextureLibrary::buildResource(
                                      const CubeTextureDescription& description)
{
  if(description.samples != VK_SAMPLE_COUNT_1_BIT) Abort("CubeTextureLibrary::_readFromFile: wrong samples number in description.");

  std::array<QImage, 6> images = _readData(description);
  uint extent = _getExtent(images);
  _checkExtent(extent, images);

  CubeTexture::SideDataInfo dataInfo[6];
  for(size_t side = 0; side < 6; side++)
  {
    if(images[side].isNull()) continue;
    dataInfo[side].data = images[side].constBits();
    dataInfo[side].datasize = images[side].sizeInBytes();
    dataInfo[side].srcRowLength = images[side].bytesPerLine() / 4;
  }

  std::shared_ptr<CubeTexture> newTexture(new CubeTexture(*description.device));

  newTexture->setData(VK_FORMAT_R8G8B8A8_SRGB,
                      images[0].width(),
                      VK_SAMPLE_COUNT_1_BIT,
                      dataInfo,
                      true);

  return newTexture;
}

std::array<QImage, 6> CubeTextureLibrary::_readData(
                                      const CubeTextureDescription& description)
{
  std::array<QImage, 6> images;

  for(size_t side = 0; side < 6; side++)
  {
    if(description.sideNames[side].isEmpty()) continue;
    QImage image(description.sideNames[side]);

    if(image.isNull() || image.width() == 0 || image.height() == 0)
    {
      std::string errorString("Unable to read image from ");
      errorString += description.sideNames[side].toLocal8Bit().data();
      throw std::runtime_error(errorString);
    }
    if(image.format() != QImage::Format_RGBA8888)
    {
      image = image.convertToFormat(QImage::Format_RGBA8888);
      if(image.isNull()) throw std::runtime_error("Unable to convert texture image to rgba format.");
    }
    images[side] = image;
  }

  return images;
}

uint CubeTextureLibrary::_getExtent(const std::array<QImage, 6>& images) const
{
  for(const QImage& image : images)
  {
    if(image.width() > 0) return image.width();
  }
  return 1;
}

void CubeTextureLibrary::_checkExtent(uint extent,
                                      std::array<QImage, 6>& images)
{
  for(QImage& image : images)
  {
    if(image.isNull())
    {
      image = QImage(extent, extent, QImage::Format_RGBA8888);
      image.setPixelColor(QPoint(0, 0), Qt::black);
    }
    else
    {
      if(image.width() != extent)
      {
        throw std::runtime_error("Images are of different sizes.");
      }
      if (image.width() != image.height())
      {
        throw std::runtime_error("Image width is not equal to height.");
      }
    }
  }
}