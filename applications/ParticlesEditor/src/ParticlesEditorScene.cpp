#include <ParticlesEditorScene.h>

ParticlesEditorScene::ParticlesEditorScene() :
  mtt::EditorScene( std::make_unique<RootObject>(tr("Effect"), false),
                    std::make_unique<mtt::EnvironmentRootObject>(
                                                              tr("Environment"),
                                                              false)),
  _dataRoot(static_cast<RootObject*>(&mtt::EditorScene::dataRoot()))
{
}
