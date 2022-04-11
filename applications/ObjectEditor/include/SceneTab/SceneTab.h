#pragma once

#include <mtt/editorLib/SceneTab/SceneTab.h>

class EditorCommonData;
class ObjectEditorScene;

class SceneTab : public mtt::SceneTab
{
  Q_OBJECT

public:
  explicit SceneTab(EditorCommonData& commonEditData);
  SceneTab(const SceneTab&) = delete;
  SceneTab& operator = (const SceneTab&) = delete;
  virtual ~SceneTab() noexcept = default;

protected:
  virtual std::unique_ptr<mtt::PropertiesWidgetFactory>
                      createWidgetsFactory(QVBoxLayout& widgetsLayout) override;

private:
  void _setScene(ObjectEditorScene* scene);

private:
  EditorCommonData& _commonEditData;
};