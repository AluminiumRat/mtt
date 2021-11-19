#include <QtCore/QObject>

#include <mtt/Render/Mesh/MeshExtraData.h>

#include <AsyncTasks/UploadTextureTask.h>
#include <EditorApplication.h>

UploadTextureTask::UploadTextureTask( const QString& filename,
                                      mtt::MeshExtraData& dstData,
                                      const char* samplerName) :
  AbstractAsyncTask(QObject::tr("Upload texture"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::SILENT),
  _filename(filename),
  _dstData(dstData),
  _samplerName(samplerName)
{
}

void UploadTextureTask::asyncPart()
{
  if(_filename.isEmpty()) return;

  mtt::Texture2DLibrary& textureLibrary =
                                  EditorApplication::instance().textureLibrary;
  mtt::LogicalDevice& device = mtt::Application::instance().displayDevice();
  std::shared_ptr<mtt::Texture2D> texture = textureLibrary.load(_filename,
                                                                device,
                                                                true);
  _sampler.reset(new mtt::Sampler(mtt::PipelineResource::STATIC,
                                  device));
  _sampler->setAttachedTexture(texture);
}

void UploadTextureTask::finalizePart()
{
  _dstData.removeSampler(_samplerName);
  if(_sampler != nullptr) _dstData.addSampler(std::move(_sampler), _samplerName);
}
