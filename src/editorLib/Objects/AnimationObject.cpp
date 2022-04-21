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

void AnimationObject::update(TimeRange time)
{
  TimeRange activeTime = _timeRange.intersection(time);
  if(!activeTime.isValid()) return;

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

TimeRange AnimationObject::calculateTiming() const noexcept
{
  TimeRange newRange;

  for(size_t childIndex = 0; childIndex < childsNumber(); childIndex++)
  {
    const AnimationTrack& track = child(childIndex);
    newRange.extend(track.timeRange());
  }

  return newRange;
}

void AnimationObject::updateTiming() noexcept
{
  TimeRange newRange = calculateTiming();
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
