#include <AsyncTasks/LoadEnvironmentTask.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

LoadEnvironmentTask::LoadEnvironmentTask( EditorScene& scene,
                                          const QString& filename,
                                          EditorCommonData& commonData) :
  mtt::LoadEnvironmentTask(filename),
  _scene(scene),
  _commonData(commonData)
{
}

void LoadEnvironmentTask::_clearScene() noexcept
{
  mtt::EnvironmentGroup& environment = _scene.root().environment();
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
      _scene.root().environment().addChild(std::move(object));
    }

    _scene.root().changeBackground(std::move(newBackground));

    _commonData.setEnvironmentFilename(filename());
  }
  catch (...)
  {
    _clearScene();
    throw;
  }
}