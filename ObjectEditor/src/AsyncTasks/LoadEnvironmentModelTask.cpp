#include <stdexcept>

#include <QtCore/QFileInfo>
#include <QtCore/QObject>

#include <mtt/Utilities/Abort.h>

#include <AsyncTasks/LoadEnvironmentModelTask.h>
#include <EditorApplication.h>

LoadEnvironmentModelTask::LoadEnvironmentModelTask( const QString& filename,
                                                    const Callback& callback) :
  AbstractAsyncTask(QObject::tr("Upload model"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::EXPLICIT),
  _filename(filename),
  _callback(callback)
{
  if(!_callback) mtt::Abort("LoadEnvironmentModelTask::LoadEnvironmentModelTask: callback is empty");
}

void LoadEnvironmentModelTask::asyncPart()
{
  mtt::LogicalDevice& device = EditorApplication::instance().displayDevice();

  QFileInfo fileInfo(_filename);
  QString extension = fileInfo.suffix();

  if(extension == "mmd")
  {
    mtt::MMDModelLibrary& library =
                                EditorApplication::instance().mmdModelLibrary;
    _model = library.load(_filename, device);
  }
  else if (extension == "fbx")
  {
    mtt::FbxModelLibrary& library =
                                  EditorApplication::instance().fbxModelLibrary;
    _model = library.load(_filename,
                          mtt::BaseFbxImporter::blenderMaterialOptions,
                          device);
  }
  else throw std::runtime_error("Model file type is not supported");
}

void LoadEnvironmentModelTask::finalizePart()
{
  _callback(std::move(_model));
}
