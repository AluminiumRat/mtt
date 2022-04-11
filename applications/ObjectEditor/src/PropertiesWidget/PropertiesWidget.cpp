#include <mtt/utilities/Abort.h>

#include <PropertiesWidget/PropertiesWidgetFactory.h>
#include <PropertiesWidget/PropertiesWidget.h>
#include <ObjectEditorCommonData.h>
#include <ObjectEditorScene.h>

PropertiesWidget::PropertiesWidget(ObjectEditorCommonData& commonEditData) :
  mtt::PropertiesWidget(commonEditData),
  _commonEditData(commonEditData)
{
}

std::unique_ptr<mtt::PropertiesWidgetFactory>
              PropertiesWidget::createWidgetsFactory(QVBoxLayout& widgetsLayout)
{
  if(_commonEditData.scene() == nullptr) mtt::Abort("PropertiesWidget::createWidgetsFactory: current scene is null.");
  return std::make_unique<PropertiesWidgetFactory>( widgetsLayout,
                                                    _commonEditData);
}
