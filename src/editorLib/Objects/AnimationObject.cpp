#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/editorLib/Objects/AnimationObject.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

AnimationObject::AnimationObject( const QString& name,
                                  bool canBeRenamed,
                                  const UID& id) :
  SpecialGroup(name, canBeRenamed, id),
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

std::unique_ptr<AbstractEditCommand> AnimationObject::makeRestoreCommand() const
{
  if(childsNumber() == 0) return nullptr;

  std::unique_ptr<CompositeCommand> resultCommand(new CompositeCommand);
  for(size_t childIndex = 0; childIndex < childsNumber(); childIndex++)
  {
    std::unique_ptr<AbstractEditCommand> childCommand =
                                        child(childIndex).makeRestoreCommand();
    if(childCommand != nullptr)
    {
      resultCommand->addSubcommand(std::move(childCommand));
    }
  }

  return resultCommand;
}
