#include <SceneTab/ParticleAnimationWidget.h>

#include <GeneratedFiles/ui_ParticleAnimationWidget.h>

ParticleAnimationWidget::ParticleAnimationWidget( ParticleAnimation& object,
                                                  mtt::UndoStack& undoStack) :
  _ui(new Ui::ParticleAnimationWidget)
{
  _ui->setupUi(this);

  _durationConnection.emplace(*_ui->durationSpin,
                              object,
                              &ParticleAnimation::duration,
                              &ParticleAnimation::setDuration,
                              &ParticleAnimation::durationChanged,
                              undoStack);
  adjustSize();
}

ParticleAnimationWidget::~ParticleAnimationWidget() noexcept = default;