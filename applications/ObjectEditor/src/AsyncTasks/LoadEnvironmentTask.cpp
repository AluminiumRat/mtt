#include <AsyncTasks/LoadEnvironmentTask.h>
#include <ObjectEditorCommonData.h>
#include <ObjectEditorScene.h>

LoadEnvironmentTask::LoadEnvironmentTask( ObjectEditorScene& scene,
                                          const QString& filename,
                                          ObjectEditorCommonData& commonData) :
  mtt::LoadEnvironmentTask(filename),
  _scene(scene),
  _commonData(commonData)
{
}

void LoadEnvironmentTask::_clearScene() noexcept
{
  mtt::EnvironmentGroup& environment = _scene.environmentRoot().objects();
  while (environment.childsNumber() != 0)
  {
    environment.removeChild(environment.child(environment.childsNumber() -1),
                            true);
  }
}

void LoadEnvironmentTask::mergeToScene(
                std::unique_ptr<mtt::BackgroundObject> newBackground,
                std::vector<std::unique_ptr<mtt::EnvironmentObject>> newObjects)
{
  _commonData.undoStack().clear();
  _commonData.setEnvironmentFilename("");
  _clearScene();

  try
  {
    for (std::unique_ptr<mtt::EnvironmentObject>& object : newObjects)
    {
      _scene.environmentRoot().objects().addChild(std::move(object));
    }

    _scene.environmentRoot().changeBackground(std::move(newBackground));

    _commonData.setEnvironmentFilename(filename());
  }
  catch (...)
  {
    _clearScene();
    throw;
  }
}