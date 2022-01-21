#include <string>

#include <QtCore/QObject>

#include <mtt/render/Pipeline/CubeTexture.h>

#include <AsyncTasks/UploadCubetextureTask.h>
#include <EditorApplication.h>

UploadCubetextureTask::UploadCubetextureTask(
                                        const std::array<QString, 6>& filenames,
                                        Callback&& callback) :
  AbstractAsyncTask(QObject::tr("Upload texture"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::SILENT),
  _filenames(filenames),
  _callback(callback)
{
}

void UploadCubetextureTask::asyncPart()
{
  mtt::CubeTextureLibrary& textureLibrary =
                              EditorApplication::instance().cubeTextureLibrary;
  mtt::LogicalDevice& device = mtt::Application::instance().displayDevice();
  _texture = textureLibrary.load(_filenames, device, true);
}

void UploadCubetextureTask::finalizePart()
{
  if(_callback != nullptr && _texture != nullptr) _callback(_texture);
}
