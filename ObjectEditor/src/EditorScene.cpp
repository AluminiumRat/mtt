#include <EditorScene.h>

EditorScene::EditorScene()
{
  registerObject(_root);
}

EditorScene::~EditorScene() noexcept
{
  startDestruction();
}
