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
  mtt::MMDModelLibrary& library =
                                EditorApplication::instance().mmdModelLibrary;
  _model = library.load(_filename, device);
}

void LoadEnvironmentModelTask::finalizePart()
{
  _callback(std::move(_model));
}
