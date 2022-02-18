#include <stdexcept>

#include <mtt/editorLib/Objects/CubemapObject.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/render/Pipeline/CubeTexture.h>
#include <mtt/utilities/Log.h>

#include <AsyncTasks/UploadCubetextureTask.h>
#include <Render/CubemapObserver.h>

CubemapObserver::CubemapObserver(mtt::CubemapObject& object) :
  _object(object)
{
  connect(&_object,
          &mtt::CubemapObject::texturesChanged,
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
    mtt::AsyncTaskQueue& queue =
                            mtt::EditorApplication::instance().asyncTaskQueue;
    _uploadStopper = queue.addTaskWithStopper(std::move(task));
  }
  catch (std::exception& error)
  {
    mtt::Log() << "CubemapObserver::_updateTexture: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "CubemapObserver::_updateTexture: unknown error.";
  }
}
