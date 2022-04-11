#pragma once

#include <mtt/editorLib/PropertiesWidget/PropertiesWidget.h>

class ParticlesEditorCommonData;
class ParticlesEditorScene;

class PropertiesWidget : public mtt::PropertiesWidget
{
  Q_OBJECT

public:
  explicit PropertiesWidget(ParticlesEditorCommonData& commonEditData);
  PropertiesWidget(const PropertiesWidget&) = delete;
  PropertiesWidget& operator = (const PropertiesWidget&) = delete;
  virtual ~PropertiesWidget() noexcept = default;

protected:
  virtual std::unique_ptr<mtt::PropertiesWidgetFactory>
                      createWidgetsFactory(QVBoxLayout& widgetsLayout) override;

private:
  ParticlesEditorCommonData& _commonEditData;
};