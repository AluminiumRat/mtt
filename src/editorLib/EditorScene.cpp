#include <mtt/editorLib/EditorScene.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

EditorScene::EditorScene(
                        std::unique_ptr<Object> dataRoot,
                        std::unique_ptr<EnvironmentRootObject> environmentRoot) :
  _dataRoot(std::move(dataRoot)),
  _environmentRoot(std::move(environmentRoot))
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
