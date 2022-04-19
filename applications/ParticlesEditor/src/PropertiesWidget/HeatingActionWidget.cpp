#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>

#include <PropertiesWidget/HeatingActionWidget.h>

#include <GeneratedFiles/ui_HeatingActionWidget.h>

HeatingActionWidget::HeatingActionWidget( HeatingAction& object,
                                          mtt::Object& targetSelectArea,
                                          mtt::UndoStack& undoStack) :
  _ui(new Ui::HeatingActionWidget)
{
  _ui->setupUi(this);

  _energyConnection.emplace(*_ui->energySpin,
                            object,
                            &HeatingAction::emittedEnergy,
                            &HeatingAction::setEmittedEnergy,
                            &HeatingAction::emittedEnergyChanged,
                            undoStack);

  using ReferenceWidget = mtt::ReferencePropertyWidget<
                                                    HeatingAction,
                                                    HeaterObject,
                                                    &HeatingAction::heaterRef>;
  ReferenceWidget* referenceWidget =
                              new ReferenceWidget(object,
                                                  &HeatingAction::heaterChanged,
                                                  undoStack,
                                                  targetSelectArea);
  _ui->targetLayout->addWidget(referenceWidget, 3);
}

HeatingActionWidget::~HeatingActionWidget() noexcept = default;
