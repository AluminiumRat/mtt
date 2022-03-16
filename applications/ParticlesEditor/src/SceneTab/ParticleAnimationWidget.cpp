#include <SceneTab/ParticleAnimationWidget.h>

#include <GeneratedFiles/ui_ParticleAnimationWidget.h>

ParticleAnimationWidget::ParticleAnimationWidget( ParticleAnimation& object,
                                                  mtt::UndoStack& undoStack) :
  _ui(new Ui::ParticleAnimationWidget)
{
  _ui->setupUi(this);

  _timeRangeConnection.emplace( *_ui->minTimeSpin,
                                *_ui->maxTimeSpin,
                                object,
                                &ParticleAnimation::timeRange,
                                &ParticleAnimation::setTimeRange,
                                &ParticleAnimation::timeRangeChanged,
                                undoStack);

  adjustSize();
}

ParticleAnimationWidget::~ParticleAnimationWidget() noexcept = default;