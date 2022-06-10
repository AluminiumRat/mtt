#include <QtCore/QObject>

#include <mtt/editorLib/AsyncTasks/UploadTextureTask.h>
#include <mtt/editorLib/EditorApplication.h>

using namespace mtt;

UploadTextureTask::UploadTextureTask( const QString& filename,
                                      const ApplyFunction& applyFunction) :
  AbstractAsyncTask(QObject::tr("Texture loading"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::SILENT),
  _filename(filename),
  _applyFunction(applyFunction)
{
}

void UploadTextureTask::asyncPart()
{
  _texture.reset();

  if(_filename.isEmpty()) return;

  Texture2DLibrary& textureLibrary =
                                  EditorApplication::instance().textureLibrary;
  LogicalDevice& device = Application::instance().displayDevice();
  _texture = textureLibrary.load(_filename, device, true);
}

void UploadTextureTask::finalizePart()
{
  if(_applyFunction) _applyFunction(std::move(_texture));
}
