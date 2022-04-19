#include <PropertiesWidget/AnimationActionWidget.h>

#include <GeneratedFiles/ui_AnimationActionWidget.h>

AnimationActionWidget::AnimationActionWidget( AnimationAction& object,
                                              mtt::UndoStack& undoStack) :
  _ui(new Ui::AnimationActionWidget)
{
  _ui->setupUi(this);

  _startTimeConnection.emplace( *_ui->startTimeSpin,
                                object,
                                &AnimationAction::startTime,
                                &AnimationAction::setStartTime,
                                &AnimationAction::startTimeChanged,
                                undoStack);

  _durationConnection.emplace(*_ui->durationSpin,
                              object,
                              &AnimationAction::duration,
                              &AnimationAction::setDuration,
                              &AnimationAction::durationChanged,
                              undoStack);
}

AnimationActionWidget::~AnimationActionWidget() noexcept = default;
