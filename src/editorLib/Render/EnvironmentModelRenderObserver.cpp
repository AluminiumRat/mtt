#include <stdexcept>

#include <mtt/editorLib/AsyncTasks/LoadEnvironmentModelTask.h>
#include <mtt/editorLib/Objects/EnvironmentModel.h>
#include <mtt/editorLib/Render/EnvironmentModelRenderObserver.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

EnvironmentModelRenderObserver::EnvironmentModelRenderObserver(
                                                  EnvironmentModel& object,
                                                  CommonEditData& commonData) :
  Object3DRenderObserver(object, commonData),
  _object(object)
{
  connect(&_object,
          &EnvironmentModel::filenameChanged,
          this,
          &EnvironmentModelRenderObserver::_updateModel,
          Qt::DirectConnection);
  _updateModel();
}

void EnvironmentModelRenderObserver::_updateModel() noexcept
{
  if(_drawModel != nullptr)
  {
    unregisterCulledDrawable(*_drawModel);
    fullTransformJoint().removeChild(*_drawModel);
    _drawModel.reset();
  }

  QString filename = _object.filename();
  if (!filename.isEmpty())
  {
    try
    {
      auto callback = [&](std::unique_ptr<SlaveDrawModel> model)
      {
        try
        {
          model->addModificator(visibleFilter());
          model->addModificator(uidSetter());
          model->addModificator(selectionModificator());
          _drawModel = std::move(model);
          registerCulledDrawable(*_drawModel);
          fullTransformJoint().addChild(*_drawModel);
        }
        catch (...)
        {
          unregisterCulledDrawable(*_drawModel);
          fullTransformJoint().removeChild(*_drawModel);
          _drawModel.reset();
          throw;
        }
      };

      std::unique_ptr<LoadEnvironmentModelTask> task(
                              new LoadEnvironmentModelTask(filename, callback));
      AsyncTaskQueue& queue = EditorApplication::instance().asyncTaskQueue;
      _uploadStopper = queue.addTaskWithStopper(std::move(task));
    }
    catch(std::exception& error)
    {
      Log() << "Unable to load model: " << error.what();
    }
    catch (...)
    {
      Log() << "Unable to load model: unknown error";
    }
  }
}
