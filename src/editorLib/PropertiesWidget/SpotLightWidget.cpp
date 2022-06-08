#include <mtt/application/Widgets/PropertiesWidgets/FilenamePropertyWidget.h>
#include <mtt/editorLib/PropertiesWidget/SpotLightWidget.h>

#include <GeneratedFiles/ui_SpotLightWidget.h>

using namespace mtt;

SpotLightWidget::SpotLightWidget( SpotLightObject& object,
                                      UndoStack& undoStack) :
  _ui(new Ui::SpotLightWidget)
{
  _ui->setupUi(this);

  _angleConnection.emplace( *_ui->angleSpin,
                            object,
                            &SpotLightObject::angle,
                            &SpotLightObject::setAngle,
                            &SpotLightObject::angleChanged,
                            undoStack);
  _angleConnection->setMultiplier(180.f / glm::pi<float>());

  using FilterWidget = FilenamePropertyWidget<SpotLightObject>;
  FilterWidget* filterWidget =
            new FilterWidget( object,
                              &SpotLightObject::filterImage,
                              &SpotLightObject::setFilterImage,
                              &SpotLightObject::filterImageChanged,
                              tr("picture files(*.png *.jpg *.jpeg *.bmp)"),
                              undoStack);
  _ui->filterLayout->addWidget(filterWidget, 3);

  _shadowsConnection.emplace( *_ui->shadowsCheckbox,
                              object,
                              &SpotLightObject::shadowsEnabled,
                              &SpotLightObject::setShadowsEnabled,
                              &SpotLightObject::shadowsEnabledChanged,
                              undoStack);

  _blurConnection.emplace(*_ui->blurSpin,
                          object,
                          &SpotLightObject::blurAngle,
                          &SpotLightObject::setBlurAngle,
                          &SpotLightObject::blurAngleChanged,
                          undoStack);
  _blurConnection->setMultiplier(180.f / glm::pi<float>());

  _mapSizeConnection.emplace( *_ui->mapSizeSpin,
                              object,
                              &SpotLightObject::shadowmapSize,
                              &SpotLightObject::setShadowmapSize,
                              &SpotLightObject::shadowmapSizeChanged,
                              undoStack);
}

SpotLightWidget::~SpotLightWidget() noexcept = default;