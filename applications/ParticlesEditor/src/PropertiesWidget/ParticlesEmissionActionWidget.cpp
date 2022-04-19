#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>

#include <PropertiesWidget/ParticlesEmissionActionWidget.h>

#include <GeneratedFiles/ui_ParticlesEmissionActionWidget.h>

ParticlesEmissionActionWidget::ParticlesEmissionActionWidget(
                                                ParticlesEmissionAction& object,
                                                mtt::Object& targetSelectArea,
                                                mtt::UndoStack& undoStack) :
  _ui(new Ui::ParticlesEmissionActionWidget)
{
  _ui->setupUi(this);

  _particlesNumberConnection.emplace(
                              *_ui->particlesNumberSpin,
                              object,
                              &ParticlesEmissionAction::particlesNumber,
                              &ParticlesEmissionAction::setParticlesNumber,
                              &ParticlesEmissionAction::particlesNumberChanged,
                              undoStack);

  using ReferenceWidget = mtt::ReferencePropertyWidget<
                                          ParticlesEmissionAction,
                                          EmitterObject,
                                          &ParticlesEmissionAction::emitterRef>;
  ReferenceWidget* referenceWidget =
                  new ReferenceWidget(object,
                                      &ParticlesEmissionAction::emitterChanged,
                                      undoStack,
                                      targetSelectArea);
  _ui->targetLayout->addWidget(referenceWidget, 3);
}

ParticlesEmissionActionWidget::
                            ~ParticlesEmissionActionWidget() noexcept = default;
