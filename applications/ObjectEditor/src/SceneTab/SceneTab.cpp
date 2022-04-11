#include <mtt/utilities/Abort.h>

#include <SceneTab/PropertiesWidgetFactory.h>
#include <SceneTab/SceneTab.h>
#include <ObjectEditorCommonData.h>
#include <ObjectEditorScene.h>

SceneTab::SceneTab(ObjectEditorCommonData& commonEditData) :
  mtt::SceneTab(commonEditData),
  _commonEditData(commonEditData)
{
  connect(&_commonEditData,
          &ObjectEditorCommonData::sceneChanged,
          this,
          &SceneTab::_setScene,
          Qt::DirectConnection);
  _setScene(_commonEditData.scene());
}

void SceneTab::_setScene(ObjectEditorScene* scene)
{
  if(scene == nullptr) setRootObject(nullptr);
  else setRootObject(&scene->dataRoot());
}

std::unique_ptr<mtt::PropertiesWidgetFactory>
                    SceneTab::createWidgetsFactory(QVBoxLayout& widgetsLayout)
{
  if(_commonEditData.scene() == nullptr) mtt::Abort("SceneTab::createWidgetsFactory: current scene is null.");
  return std::make_unique<PropertiesWidgetFactory>( widgetsLayout,
                                                    _commonEditData);
}
