#include <mtt/editorLib/Objects/AnimationTrack.h>

using namespace mtt;

AnimationTrack::AnimationTrack( const QString& name,
                                bool canBeRenamed,
                                const UID& id) :
  Object(name, canBeRenamed,id),
  _enabled(true),
  _timeRange(TimeT(0), TimeT(0))
{
}

void AnimationTrack::setEnabled(bool newValue)
{
  if(newValue == _enabled) return;
  _enabled = newValue;
  emit enabledChanged(_enabled);
}

void AnimationTrack::setTimeRange(const TimeRange& newValue) noexcept
{
  if(newValue == _timeRange) return;
  _timeRange = newValue;
  emit timeRangeChanged(newValue);
}
