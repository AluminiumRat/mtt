#include <stdexcept>

#include <QtGui/QImage>

#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

std::shared_ptr<Texture2D> Texture2DLibrary::load(const QString& filename,
                                                  LogicalDevice& device,
                                                  bool generateLods)
{
  Texture2DDescription description{ filename,
                                    &device,
                                    VK_SAMPLE_COUNT_1_BIT,
                                    generateLods};
  return getOrCreate(description);
}

std::unique_ptr<Texture2D> Texture2DLibrary::loadTexture(
                                                        const QString& filename,
                                                        LogicalDevice& device,
                                                        bool generateLods)
{
  QImage image(filename);
  if(image.isNull() || image.width() == 0 || image.height() == 0)
  {
    std::string errorString("Unable to read image from ");
    errorString += filename.toLocal8Bit().data();
    throw std::runtime_error(errorString);
  }
  if(image.format() != QImage::Format_RGBA8888)
  {
    image = image.convertToFormat(QImage::Format_RGBA8888);
    if(image.isNull()) throw std::runtime_error("Unable to convert texture image to rgba format.");
  }

  std::unique_ptr<Texture2D> newTexture(new Texture2D(device));

  newTexture->setData(image.constBits(),
                      image.sizeInBytes(),
                      VK_FORMAT_R8G8B8A8_SRGB,
                      glm::uvec2(image.width(), image.height()),
                      VK_SAMPLE_COUNT_1_BIT,
                      image.bytesPerLine() / 4,
                      generateLods);

  return newTexture;
}

std::shared_ptr<Texture2D> Texture2DLibrary::buildResource(
                                        const Texture2DDescription& description)
{
  if(description.samples != VK_SAMPLE_COUNT_1_BIT) Abort("Texture2DLibrary::_readFromFile: wrong samples number in description.");
  return std::shared_ptr<Texture2D>(loadTexture(description.name,
                                                *description.device,
                                                description.lods));
}
