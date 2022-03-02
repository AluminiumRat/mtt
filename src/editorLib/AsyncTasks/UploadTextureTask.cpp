#include <QtCore/QObject>

#include <mtt/editorLib/AsyncTasks/UploadTextureTask.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/Mesh/MeshExtraData.h>

using namespace mtt;

UploadTextureTask::UploadTextureTask( const QString& filename,
                                      MeshExtraData& dstData,
                                      const char* samplerName) :
  AbstractAsyncTask(QObject::tr("Texture loading"),
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

  Texture2DLibrary& textureLibrary =
                                  EditorApplication::instance().textureLibrary;
  LogicalDevice& device = Application::instance().displayDevice();
  std::shared_ptr<Texture2D> texture = textureLibrary.load( _filename,
                                                            device,
                                                            true);
  _sampler.reset(new Sampler(PipelineResource::STATIC, device));
  _sampler->setAttachedTexture(texture);
}

void UploadTextureTask::finalizePart()
{
  _dstData.removeSampler(_samplerName);
  if(_sampler != nullptr) _dstData.addSampler(std::move(_sampler), _samplerName);
}
