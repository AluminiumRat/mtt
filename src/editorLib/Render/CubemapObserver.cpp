#include <stdexcept>

#include <mtt/editorLib/AsyncTasks/UploadCubetextureTask.h>
#include <mtt/editorLib/Objects/CubemapObject.h>
#include <mtt/editorLib/Render/CubemapObserver.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/Pipeline/CubeTexture.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

CubemapObserver::CubemapObserver(CubemapObject& object) :
  _object(object)
{
  connect(&_object,
          &CubemapObject::texturesChanged,
          this,
          &CubemapObserver::_updateTexture,
          Qt::DirectConnection);
}

void CubemapObserver::setCallback(Callback&& newCallback) noexcept
{
  _callback = newCallback;
  _updateTexture();
}

void CubemapObserver::_updateTexture() noexcept
{
  try
  {
    std::array<QString, 6> filenames = _object.textures();
    if( filenames[0].isEmpty() &&
        filenames[1].isEmpty() &&
        filenames[2].isEmpty() &&
        filenames[3].isEmpty() &&
        filenames[4].isEmpty() &&
        filenames[5].isEmpty())
    {
      _uploadStopper.reset();
      if(_callback != nullptr) _callback(nullptr);
      return;
    }

    std::unique_ptr<UploadCubetextureTask> task;
    task.reset(new UploadCubetextureTask( filenames,
                                          std::move(Callback(_callback))));
    AsyncTaskQueue& queue = EditorApplication::instance().asyncTaskQueue;
    _uploadStopper = queue.addTaskWithStopper(std::move(task));
  }
  catch (std::exception& error)
  {
    Log() << "CubemapObserver::_updateTexture: " << error.what();
  }
  catch (...)
  {
    Log() << "CubemapObserver::_updateTexture: unknown error.";
  }
}
