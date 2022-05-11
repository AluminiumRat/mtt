#include <ParticlesEditorScene.h>

ParticlesEditorScene::ParticlesEditorScene() :
  mtt::EditorScene( std::make_unique<RootObject>(tr("Effect"), false)),
  _dataRoot(static_cast<RootObject*>(&mtt::EditorScene::dataRoot()))
{
}
