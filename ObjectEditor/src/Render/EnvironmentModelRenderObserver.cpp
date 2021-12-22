#include <stdexcept>

#include <mtt/Application/DrawModel/DrawModelLoader.h>
#include <mtt/DrawImpl/MeshTechniques/ModelTechniquesFactory.h>
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
      mtt::ModelTechniquesFactory techniqueFactory(
                                          true,
                                          true,
                                          true,
                                          true,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
      mtt::DrawModelLoader loader(
                                filename,
                                techniqueFactory,
                                EditorApplication::instance().textureLibrary,
                                EditorApplication::instance().displayDevice());
      std::unique_ptr<mtt::DrawModel> newModel = loader.load();
      newModel->registedModificator(visibleFilter());
      newModel->registedModificator(uidSetter());
      newModel->registedModificator(selectionModificator());
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
