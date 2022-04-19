#include <Objects/AnimationAction.h>

AnimationAction::AnimationAction( const QString& name,
                                  bool canBeRenamed,
                                  const mtt::UID& id) :
  AnimationTrack(name, canBeRenamed, id)
{
  setTimeRange(TimeRange(_startTime, _startTime + _duration));
}

void AnimationAction::setStartTime(mtt::TimeT newValue) noexcept
{
  if(newValue.count() < 0) newValue = mtt::TimeT(0);
  if(_startTime == newValue) return;
  _startTime = newValue;
  setTimeRange(TimeRange(_startTime, _startTime + _duration));
  emit startTimeChanged(newValue);
}

void AnimationAction::setDuration(mtt::TimeT newValue) noexcept
{
  if(newValue.count() < 0) newValue = mtt::TimeT(0);
  if(_duration == newValue) return;
  _duration = newValue;
  setTimeRange(TimeRange(_startTime, _startTime + _duration));
  emit durationChanged(newValue);
}

void AnimationAction::update(TimeRange time)
{
  if(!enabled()) return;
  TimeRange activeTime = timeRange().intersection(time);
  if(!activeTime.isValid()) return;
  if(timeRange().length() == mtt::TimeT(0)) makeAction(1.f);
  else makeAction(float(activeTime.length().count()) /
                                          float(timeRange().length().count()));
}

std::unique_ptr<mtt::AbstractEditCommand>
                                    AnimationAction::makeRestoreCommand() const
{
  return nullptr;
}
