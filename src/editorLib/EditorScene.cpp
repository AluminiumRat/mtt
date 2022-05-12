#include <mtt/editorLib/EditorScene.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

EditorScene::EditorScene(std::unique_ptr<Object> dataRoot) :
  _dataRoot(std::move(dataRoot)),
  _environmentRoot(new EnvironmentRootObject(tr("Environment"), false))
{
  if(_dataRoot == nullptr) Abort("EditorScene::EditorScene: dataRoot is nullptr");
  registerObject(*_dataRoot);

  if(_environmentRoot == nullptr) Abort("EditorScene::EditorScene: environmentRoot is nullptr");
  registerObject(*_environmentRoot);
}

EditorScene::~EditorScene() noexcept
{
  startDestruction();
}

void EditorScene::changeEnvironmentRoot(
                        std::unique_ptr<EnvironmentRootObject> newRoot) noexcept
{
  if(newRoot == nullptr) Abort("EditorScene::changeEnvironmentRoot: newRoot is nullpt.");

  std::unique_ptr<EnvironmentRootObject> oldRoot =
                                                    std::move(_environmentRoot);
  try
  {
    _environmentRoot = std::move(newRoot);
    registerObject(*_environmentRoot);
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("EditorScene::changeEnvironmentRoot: unable to change environment root object.");
  }
  catch (...)
  {
    Abort("EditorScene::changeEnvironmentRoot: unable to change environment root object.");
  }

  emit environmentRootChnaged(*_environmentRoot);
}

std::unique_ptr<Object> EditorScene::changeDataRoot(
                                      std::unique_ptr<Object> newRoot) noexcept
{
  if(newRoot == nullptr) Abort("EditorScene::changeDataRoot: newRoot is nullpt.");

  std::unique_ptr<Object> oldRoot =std::move(_dataRoot);
  try
  {
    _dataRoot = std::move(newRoot);
    registerObject(*_dataRoot);
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("EditorScene::changeDataRoot: unable to change data root object.");
  }
  catch (...)
  {
    Abort("EditorScene::changeDataRoot: unable to change data root object.");
  }

  emit dataRootChanged(*_dataRoot);
  return oldRoot;
}
