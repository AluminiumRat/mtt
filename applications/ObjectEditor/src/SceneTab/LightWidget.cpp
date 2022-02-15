#include <mtt/application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>

#include <SceneTab/LightWidget.h>

#include <GeneratedFiles/ui_LightWidget.h>

LightWidget::LightWidget(mtt::LightObject& object, mtt::UndoStack& undoStack) :
  _ui(new Ui::LightWidget)
{
  _ui->setupUi(this);

  _enabledConnection.emplace( *_ui->enabledCheck,
                              object,
                              &mtt::LightObject::enabled,
                              &mtt::LightObject::setEnabled,
                              &mtt::LightObject::enabledChanged,
                              undoStack);

  mtt::ColorPropertyWidget<mtt::LightObject>* colorWidget =
                  new mtt::ColorPropertyWidget<mtt::LightObject>(
                                                object,
                                                &mtt::LightObject::color,
                                                &mtt::LightObject::setColor,
                                                &mtt::LightObject::colorChanged,
                                                undoStack);
  _ui->colorLayout->addWidget(colorWidget, 3);

  _powerConnection.emplace( *_ui->powerSpin,
                            object,
                            &mtt::LightObject::baseIlluminance,
                            &mtt::LightObject::setBaseIlluminance,
                            &mtt::LightObject::baseIlluminanceChanged,
                            undoStack);

  _distanceConnection.emplace(*_ui->distanceSpin,
                              object,
                              &mtt::LightObject::distance,
                              &mtt::LightObject::setDistance,
                              &mtt::LightObject::distanceChanged,
                              undoStack);

  adjustSize();
}

LightWidget::~LightWidget() noexcept = default;;
