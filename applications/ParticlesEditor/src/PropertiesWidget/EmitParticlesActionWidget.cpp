#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>

#include <PropertiesWidget/EmitParticlesActionWidget.h>

#include <GeneratedFiles/ui_EmitParticlesActionWidget.h>

EmitParticlesActionWidget::EmitParticlesActionWidget(
                                                  EmitParticlesAction& object,
                                                  mtt::Object& targetSelectArea,
                                                  mtt::UndoStack& undoStack) :
  _ui(new Ui::EmitParticlesActionWidget)
{
  _ui->setupUi(this);

  _particlesNumberConnection.emplace(
                                  *_ui->particlesNumberSpin,
                                  object,
                                  &EmitParticlesAction::particlesNumber,
                                  &EmitParticlesAction::setParticlesNumber,
                                  &EmitParticlesAction::particlesNumberChanged,
                                  undoStack);

  using ReferenceWidget = mtt::ReferencePropertyWidget<
                                              EmitParticlesAction,
                                              EmitterObject,
                                              &EmitParticlesAction::emitterRef>;
  ReferenceWidget* referenceWidget =
                      new ReferenceWidget(object,
                                          &EmitParticlesAction::emitterChanged,
                                          undoStack,
                                          targetSelectArea);
  _ui->targetLayout->addWidget(referenceWidget, 3);
}

EmitParticlesActionWidget::~EmitParticlesActionWidget() noexcept = default;
