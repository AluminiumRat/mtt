#pragma once

#include <mtt/editorLib/SceneTab/SceneTab.h>

class ParticlesEditorCommonData;
class ParticlesEditorScene;

class SceneTab : public mtt::SceneTab
{
  Q_OBJECT

public:
  explicit SceneTab(ParticlesEditorCommonData& commonEditData);
  SceneTab(const SceneTab&) = delete;
  SceneTab& operator = (const SceneTab&) = delete;
  virtual ~SceneTab() noexcept = default;

protected:
  virtual std::unique_ptr<mtt::PropertiesWidgetFactory>
                      createWidgetsFactory(QVBoxLayout& widgetsLayout) override;

private:
  void _setScene(ParticlesEditorScene* scene);

private:
  ParticlesEditorCommonData& _commonEditData;
};