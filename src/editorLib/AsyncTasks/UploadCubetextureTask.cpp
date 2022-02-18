#include <string>

#include <QtCore/QObject>

#include <mtt/editorLib/AsyncTasks/UploadCubetextureTask.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/Pipeline/CubeTexture.h>

using namespace mtt;

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
  CubeTextureLibrary& textureLibrary =
                              EditorApplication::instance().cubeTextureLibrary;
  LogicalDevice& device = Application::instance().displayDevice();
  _texture = textureLibrary.load(_filenames, device, true);
}

void UploadCubetextureTask::finalizePart()
{
  if(_callback != nullptr && _texture != nullptr) _callback(_texture);
}
