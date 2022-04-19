#include <PropertiesWidget/ActionAnimationTrackWidget.h>

#include <GeneratedFiles/ui_ActionAnimationTrackWidget.h>

ActionAnimationTrackWidget::ActionAnimationTrackWidget(
                                                  ActionAnimationTrack& object,
                                                  mtt::UndoStack& undoStack) :
  _ui(new Ui::ActionAnimationTrackWidget)
{
  _ui->setupUi(this);

  _startTimeConnection.emplace( *_ui->startTimeSpin,
                                object,
                                &ActionAnimationTrack::startTime,
                                &ActionAnimationTrack::setStartTime,
                                &ActionAnimationTrack::startTimeChanged,
                                undoStack);

  _durationConnection.emplace(*_ui->durationSpin,
                              object,
                              &ActionAnimationTrack::duration,
                              &ActionAnimationTrack::setDuration,
                              &ActionAnimationTrack::durationChanged,
                              undoStack);
}

ActionAnimationTrackWidget::~ActionAnimationTrackWidget() noexcept = default;
