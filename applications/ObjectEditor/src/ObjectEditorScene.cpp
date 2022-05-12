#include <ObjectEditorScene.h>

ObjectEditorScene::ObjectEditorScene() :
  EditorScene(std::make_unique<RootObject>(tr("Model"), false)),
  _dataRoot(static_cast<RootObject*>(&mtt::EditorScene::dataRoot()))
{
}

void ObjectEditorScene::changeDataRoot(
                                  std::unique_ptr<RootObject> newRoot) noexcept
{
  std::unique_ptr<mtt::Object> oldRoot =
                          mtt::EditorScene::changeDataRoot(std::move(newRoot));
  _dataRoot = static_cast<RootObject*>(&mtt::EditorScene::dataRoot());
  emit dataRootChanged(*_dataRoot);
}
