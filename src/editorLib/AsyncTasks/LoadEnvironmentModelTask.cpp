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

  DrawModelLibrary& library = EditorApplication::instance().drawModelLibrary;
  _model = library.load(_filename, device);
}

void LoadEnvironmentModelTask::finalizePart()
{
  _callback(std::move(_model));
}
