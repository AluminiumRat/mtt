#include <stdexcept>

#include <QtCore/QFileInfo>
#include <QtCore/QObject>

#include <mtt/editorLib/AsyncTasks/LoadEnvironmentModelTask.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

LoadEnvironmentModelTask::LoadEnvironmentModelTask( const QString& filename,
                                                    const Callback& callback) :
  AbstractAsyncTask(QObject::tr("Upload model"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::EXPLICIT),
  _filename(filename),
  _callback(callback)
{
  if(!_callback) Abort("LoadEnvironmentModelTask::LoadEnvironmentModelTask: callback is empty");
}

void LoadEnvironmentModelTask::asyncPart()
{
  LogicalDevice& device = EditorApplication::instance().displayDevice();

  QFileInfo fileInfo(_filename);
  QString extension = fileInfo.suffix();

  if(extension == "mmd")
  {
    MMDModelLibrary& library = EditorApplication::instance().mmdModelLibrary;
    _model = library.load(_filename, device);
  }
  else if (extension == "fbx")
  {
    FbxModelLibrary& library = EditorApplication::instance().fbxModelLibrary;
    _model = library.load(_filename,
                          BaseFbxImporter::blenderMaterialOptions,
                          device);
  }
  else throw std::runtime_error("Model file type is not supported");
}

void LoadEnvironmentModelTask::finalizePart()
{
  _callback(std::move(_model));
}
