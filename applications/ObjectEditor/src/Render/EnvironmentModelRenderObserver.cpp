#include <stdexcept>

#include <mtt/editorLib/Objects/EnvironmentModel.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/utilities/Log.h>

#include <AsyncTasks/LoadEnvironmentModelTask.h>
#include <Render/EnvironmentModelRenderObserver.h>

EnvironmentModelRenderObserver::EnvironmentModelRenderObserver(
                                                mtt::EnvironmentModel& object,
                                                EditorCommonData& commonData) :
  Object3DRenderObserver(object, commonData),
  _object(object)
{
  connect(&_object,
          &mtt::EnvironmentModel::filenameChanged,
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
      auto callback = [&](std::unique_ptr<mtt::SlaveDrawModel> model)
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
      mtt::AsyncTaskQueue& queue =
                              mtt::EditorApplication::instance().asyncTaskQueue;
      _uploadStopper = queue.addTaskWithStopper(std::move(task));
    }
    catch(std::exception& error)
    {
      mtt::Log() << "Unable to load model: " << error.what();
    }
    catch (...)
    {
      mtt::Log() << "Unable to load model: unknown error";
    }
  }
}
