#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>

#include <PropertiesWidget/GasEmissionActionWidget.h>

#include <GeneratedFiles/ui_GasEmissionActionWidget.h>

GasEmissionActionWidget::GasEmissionActionWidget( GasEmissionAction& object,
                                                  mtt::Object& targetSelectArea,
                                                  mtt::UndoStack& undoStack) :
  _ui(new Ui::GasEmissionActionWidget)
{
  _ui->setupUi(this);

  _volumeConnection.emplace(*_ui->volumeSpin,
                            object,
                            &GasEmissionAction::emittedVolume,
                            &GasEmissionAction::setEmittedVolume,
                            &GasEmissionAction::emittedVolumeChanged,
                            undoStack);

  using ReferenceWidget = mtt::ReferencePropertyWidget<
                                            GasEmissionAction,
                                            GasSource,
                                            &GasEmissionAction::gasSourceRef>;
  ReferenceWidget* referenceWidget =
                      new ReferenceWidget(object,
                                          &GasEmissionAction::gasSourceChanged,
                                          undoStack,
                                          targetSelectArea);
  _ui->targetLayout->addWidget(referenceWidget, 3);
}

GasEmissionActionWidget::~GasEmissionActionWidget() noexcept = default;
