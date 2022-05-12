#include <ParticlesEditorScene.h>

ParticlesEditorScene::ParticlesEditorScene() :
  mtt::EditorScene( std::make_unique<RootObject>(tr("Effect"), false)),
  _dataRoot(static_cast<RootObject*>(&mtt::EditorScene::dataRoot()))
{
}

void ParticlesEditorScene::changeDataRoot(
                                  std::unique_ptr<RootObject> newRoot) noexcept
{
  std::unique_ptr<mtt::Object> oldRoot =
                          mtt::EditorScene::changeDataRoot(std::move(newRoot));
  _dataRoot = static_cast<RootObject*>(&mtt::EditorScene::dataRoot());
  emit dataRootChanged(*_dataRoot);
}
