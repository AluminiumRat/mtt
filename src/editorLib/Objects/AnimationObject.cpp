#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/editorLib/Objects/AnimationObject.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

AnimationObject::AnimationObject( const QString& name,
                                  bool canBeRenamed,
                                  const UID& id) :
  SpecialGroup(name, canBeRenamed, id)
{
}

void AnimationObject::update(TimeT time)
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
          &AnimationTrack::timeRangeChanged,
          this,
          &AnimationObject::updateTiming,
          Qt::DirectConnection);

  Object::onSubobjectAdded(object);

  updateTiming();
}

void AnimationObject::onSubobjectRemoved(Object& object) noexcept
{
  AnimationTrack& track = static_cast<AnimationTrack&>(object);
  disconnect( &track,
              &AnimationTrack::timeRangeChanged,
              this,
              &AnimationObject::updateTiming);

  Object::onSubobjectRemoved(object);

  updateTiming();
}

Range<TimeT> AnimationObject::calculateTiming() const noexcept
{
  if (childsNumber() == 0) return Range<TimeT>();

  TimeT newStartTime = child(0).startTime();
  TimeT newFinishTime = child(0).finishTime();

  for(size_t childIndex = 1; childIndex < childsNumber(); childIndex++)
  {
    const AnimationTrack& track = child(childIndex);
    newStartTime = std::min(newStartTime, track.startTime());
    newFinishTime = std::max(newFinishTime, track.finishTime());
  }

  return Range<TimeT>(newStartTime, newFinishTime);
}

void AnimationObject::updateTiming() noexcept
{
  Range<TimeT> newRange = calculateTiming();
  if (newRange == _timeRange) return;
  _timeRange = newRange;
  emit timeRangeChanged(_timeRange);
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
