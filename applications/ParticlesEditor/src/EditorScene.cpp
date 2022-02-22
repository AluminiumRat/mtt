#include <EditorScene.h>

EditorScene::EditorScene() :
  _root(tr("Root"), false)
{
  registerObject(_root);
}

EditorScene::~EditorScene() noexcept
{
  startDestruction();
}
