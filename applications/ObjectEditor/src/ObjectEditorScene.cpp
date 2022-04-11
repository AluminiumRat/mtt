#include <ObjectEditorScene.h>

ObjectEditorScene::ObjectEditorScene() :
  EditorScene(std::make_unique<RootObject>(tr("Model"), false),
              std::make_unique<mtt::EnvironmentRootObject>( tr("Environment"),
                                                            false)),
  _dataRoot(static_cast<RootObject*>(&mtt::EditorScene::dataRoot()))
{
}
