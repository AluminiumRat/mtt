#include <mtt/application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>
#include <mtt/editorLib/PropertiesWidget/LightWidget.h>

#include <GeneratedFiles/ui_LightWidget.h>

using namespace mtt;

LightWidget::LightWidget(LightObject& object, UndoStack& undoStack) :
  _ui(new Ui::LightWidget)
{
  _ui->setupUi(this);

  _enabledConnection.emplace( *_ui->enabledCheck,
                              object,
                              &LightObject::enabled,
                              &LightObject::setEnabled,
                              &LightObject::enabledChanged,
                              undoStack);

  ColorPropertyWidget<LightObject>* colorWidget =
                  new ColorPropertyWidget<LightObject>(
                                                    object,
                                                    &LightObject::color,
                                                    &LightObject::setColor,
                                                    &LightObject::colorChanged,
                                                    undoStack);
  _ui->colorLayout->addWidget(colorWidget, 3);

  _powerConnection.emplace( *_ui->powerSpin,
                            object,
                            &LightObject::baseIlluminance,
                            &LightObject::setBaseIlluminance,
                            &LightObject::baseIlluminanceChanged,
                            undoStack);

  _distanceConnection.emplace(*_ui->distanceSpin,
                              object,
                              &LightObject::distance,
                              &LightObject::setDistance,
                              &LightObject::distanceChanged,
                              undoStack);

  adjustSize();
}

LightWidget::~LightWidget() noexcept = default;;
