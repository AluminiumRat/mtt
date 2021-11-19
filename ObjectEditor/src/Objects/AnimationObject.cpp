#include <mtt/Core/EditCommands/CompositeCommand.h>
#include <mtt/Utilities/Log.h>

#include <Objects/AnimationObject.h>

AnimationObject::AnimationObject(const mtt::UID& id) :
  SpecialGroup(id),
  _startTime(0),
  _finishTime(0)
{
}

void AnimationObject::update(TimeType time)
{
  if(time < startTime() || time > finishTime()) return;
  for(size_t trackIndex = 0; trackIndex < childsNumber(); trackIndex++)
  {
    child(trackIndex).update(time);
  }
}

void AnimationObject::onSubobjectAdded(Object& object) noexcept
{
  AnimationTrack& track = static_cast<AnimationTrack&>(object);
  connect(&track,
          &AnimationTrack::timingChanged,
          this,
          &AnimationObject::_updateTiming,
          Qt::DirectConnection);

  Object::onSubobjectAdded(object);

  _updateTiming();
}

void AnimationObject::onSubobjectRemoved(Object& object) noexcept
{
  AnimationTrack& track = static_cast<AnimationTrack&>(object);
  disconnect( &track,
              &AnimationTrack::timingChanged,
              this,
              &AnimationObject::_updateTiming);

  Object::onSubobjectRemoved(object);

  _updateTiming();
}

void AnimationObject::_updateTiming() noexcept
{
  TimeType newStartTime(0);
  TimeType newFinishTime(0);
  if(childsNumber() != 0)
  {
    newStartTime = child(0).startTime();
    newFinishTime = child(0).finishTime();
  }

  for(size_t childIndex = 1; childIndex < childsNumber(); childIndex++)
  {
    AnimationTrack& track = child(childIndex);
    newStartTime = std::min(newStartTime, track.startTime());
    newFinishTime = std::max(newFinishTime, track.finishTime());
  }

  if (newStartTime == _startTime && newFinishTime == _finishTime) return;

  TimeType oldStartTime = _startTime;
  TimeType oldFinishTime = _finishTime;

  _startTime = newStartTime;
  _finishTime = newFinishTime;

  if(oldStartTime != _startTime) emit startTimeChanged(_startTime);
  if(oldFinishTime != _finishTime) emit finishTimeChanged(_finishTime);
  emit durationChanged(duration());
  emit timingChanged();
}

std::unique_ptr<mtt::AbstractEditCommand>
                                    AnimationObject::makeRestoreCommand() const
{
  if(childsNumber() == 0) return nullptr;

  std::unique_ptr<mtt::CompositeCommand> resultCommand(
                                                    new mtt::CompositeCommand);
  for(size_t childIndex = 0; childIndex < childsNumber(); childIndex++)
  {
    std::unique_ptr<mtt::AbstractEditCommand> childCommand =
                                        child(childIndex).makeRestoreCommand();
    if(childCommand != nullptr)
    {
      resultCommand->addSubcommand(std::move(childCommand));
    }
  }

  return resultCommand;
}
