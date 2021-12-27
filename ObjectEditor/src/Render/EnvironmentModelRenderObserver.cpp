#include <stdexcept>

#include <mtt/Utilities/Log.h>

#include <Objects/EnvironmentModel.h>
#include <Render/EnvironmentModelRenderObserver.h>
#include <EditorApplication.h>

EnvironmentModelRenderObserver::EnvironmentModelRenderObserver(
                                                EnvironmentModel& object,
                                                EditorCommonData& commonData) :
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
      mtt::LogicalDevice& device =
                                  EditorApplication::instance().displayDevice();
      mtt::MMDModelLibrary& library =
                                  EditorApplication::instance().mmdModelLibrary;
      std::unique_ptr<mtt::SlaveDrawModel> newModel =
                                                library.load(filename, device);
      newModel->registerModificator(visibleFilter());
      newModel->registerModificator(uidSetter());
      newModel->registerModificator(selectionModificator());
      _drawModel = std::move(newModel);
      registerCulledDrawable(*_drawModel);
      fullTransformJoint().addChild(*_drawModel);
    }
    catch(std::exception& error)
    {
      unregisterCulledDrawable(*_drawModel);
      fullTransformJoint().removeChild(*_drawModel);
      _drawModel.reset();
      mtt::Log() << "Unable to load model: " << error.what();
    }
    catch (...)
    {
      unregisterCulledDrawable(*_drawModel);
      fullTransformJoint().removeChild(*_drawModel);
      _drawModel.reset();
      mtt::Log() << "Unable to load model: unknown error";
    }
  }
}
