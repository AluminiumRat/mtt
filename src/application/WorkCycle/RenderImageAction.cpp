#include <vector>

#include <mtt/application/WorkCycle/RenderImageAction.h>
#include <mtt/render/SceneRenderer/SceneToImageRenderer.h>
#include <mtt/render/LogicalDevice.h>

using namespace mtt;

RenderImageAction::RenderImageAction( SceneToImageRenderer& renderer,
                                      RenderScene& scene,
                                      CameraNode& camera,
                                      ViewInfo* rootViewInfo,
                                      const glm::uvec2& extent,
                                      VkImageAspectFlagBits imageAspect,
                                      const Callback& callback) :
  RenderSceneAction(renderer, scene, camera, rootViewInfo),
  _renderer(renderer),
  _extent(extent),
  _imageAspect(imageAspect),
  _callback(callback)
{
}

void RenderImageAction::asycPart()
{
  _renderer.setExtent(_extent);
  RenderSceneAction::asycPart();

  _downloadBuffer.reset();

  Image* image = _renderer.targetImage();
  if(image == nullptr) return;

  LogicalDevice& device = image->device();
  size_t bufferSize = image->memorySize();

  if(bufferSize == 0) return;

  _downloadBuffer = new PlainBuffer(device,
                                    bufferSize,
                                    PlainBuffer::DOWNLOAD_BUFFER);

  auto transferCommand =
    [&](CommandProducer& uploadProducer)
    {
      Image::CopyToBufferChunk copyInfo;
      copyInfo.srcArrayIndex = 0;
      copyInfo.srcMipLevel = 0;
      copyInfo.srcOffset = glm::uvec3(0,0,0);
      copyInfo.srcExtent = glm::uvec3(_extent, 1);
      copyInfo.dstBufferOffset = 0;
      copyInfo.dstRowLength = _extent.x;
      copyInfo.dstImageHeight = 0;
      std::vector<Image::CopyToBufferChunk> infoVec{copyInfo};
      image->scheduleCopy(uploadProducer,
                          *_downloadBuffer,
                          _imageAspect,
                          infoVec);
    };
  device.runTransferCommand(transferCommand);
}

void RenderImageAction::postAsycPart()
{
  if(_downloadBuffer == nullptr) return;
  if(!_callback) return;

  PlainBuffer::MemoryMapper mapper(*_downloadBuffer);
  _callback(mapper.data());
}
