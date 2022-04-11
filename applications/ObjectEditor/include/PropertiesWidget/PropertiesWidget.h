#pragma once

#include <mtt/editorLib/PropertiesWidget/PropertiesWidget.h>

class ObjectEditorCommonData;
class ObjectEditorScene;

class PropertiesWidget : public mtt::PropertiesWidget
{
  Q_OBJECT

public:
  explicit PropertiesWidget(ObjectEditorCommonData& commonEditData);
  PropertiesWidget(const PropertiesWidget&) = delete;
  PropertiesWidget& operator = (const PropertiesWidget&) = delete;
  virtual ~PropertiesWidget() noexcept = default;

protected:
  virtual std::unique_ptr<mtt::PropertiesWidgetFactory>
                      createWidgetsFactory(QVBoxLayout& widgetsLayout) override;

private:
  ObjectEditorCommonData& _commonEditData;
};