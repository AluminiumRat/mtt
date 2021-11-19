#include <mtt/Application/EditCommands/CompositeCommand.h>
#include <mtt/Application/EditCommands/SetPropertyCommand.h>
#include <mtt/Utilities/Abort.h>

#include <Objects/AnimationTrack.h>

AnimationTrack::AnimationTrack(const mtt::UID& id) :
  Object(id),
  _enabled(true),
  _positionAnimation(glm::vec3(0)),
  _rotationAnimation(glm::quat()),
  _scaleAnimation(glm::vec3(1)),
  _startTime(0),
  _finishTime(0)
{
  _skeletonLink.emplace(mtt::UID(), *this);
}

void AnimationTrack::setEnabled(bool newValue)
{
  if(newValue == _enabled) return;
  _enabled = newValue;
  emit enabledChanged(_enabled);
}

void AnimationTrack::update(TimeType time)
{
  if(!enabled()) return;
  if(time < startTime() || time > finishTime()) return;

  SkeletonObject* skeletonObject = skeleton();
  if(skeletonObject == nullptr) return;

  skeletonObject->setPosition(_positionAnimation.value(time));
  skeletonObject->setRotation(_rotationAnimation.value(time));
  skeletonObject->setScale(_scaleAnimation.value(time));
}

void AnimationTrack::_updateTiming() noexcept
{
  TimeType newStartTime = std::min(_positionAnimation.startTime(),
                                    _rotationAnimation.startTime());
  newStartTime = std::min(newStartTime, _scaleAnimation.startTime());

  TimeType newFinishTime = std::max(_positionAnimation.finishTime(),
                                    _rotationAnimation.finishTime());
  newFinishTime = std::max(newFinishTime, _scaleAnimation.finishTime());

  if(newStartTime == _startTime && newFinishTime == _finishTime) return;

  TimeType oldStartTime = _startTime;
  TimeType oldFinishTime = _finishTime;

  _startTime = newStartTime;
  _finishTime = newFinishTime;

  if (oldStartTime != _startTime) emit startTimeChanged(_startTime);
  if (oldFinishTime != _finishTime) emit finishTimeChanged(_finishTime);
  emit durationChanged(duration());
  emit timingChanged();
}

void AnimationTrack::setSkeleton(SkeletonObject* skeleton)
{
  if(skeleton == nullptr) setSkeletonId(mtt::UID());
  else setSkeletonId(skeleton->id());
}

void AnimationTrack::setSkeletonId(const mtt::UID& id)
{
  try
  {
    _skeletonLink.emplace(id, *this);
  }
  catch(...)
  {
    try
    {
      _skeletonLink.emplace(mtt::UID(), *this);
    }
    catch(...)
    {
      mtt::Abort("AnimationTrack::setSkeletonId: unable to emplace skeleton link");
    }
    throw;
  }
}

void AnimationTrack::_connectSkeleton(SkeletonObject& skeleton)
{
  emit skeletonRefChanged(&skeleton);
}

void AnimationTrack::_disconnectSkeleton(SkeletonObject& skeleton) noexcept
{
  emit skeletonRefChanged(nullptr);
}

std::unique_ptr<mtt::AbstractEditCommand>
                                    AnimationTrack::makeRestoreCommand() const
{
  SkeletonObject* skeletonObject = skeleton();
  if(skeletonObject == nullptr) return nullptr;

  using PositionSetter = void (SkeletonObject::*)(const glm::vec3&);
  using PositionCommand = mtt::SetPropertyCommand<SkeletonObject,
                                                  glm::vec3,
                                                  PositionSetter>;
  std::unique_ptr<PositionCommand> positionCommand(
                              new PositionCommand(*skeletonObject,
                                                  &SkeletonObject::setPosition,
                                                  skeletonObject->position(),
                                                  skeletonObject->position()));

  using RotationSetter = void (SkeletonObject::*)(const glm::quat&);
  using RotationCommand = mtt::SetPropertyCommand<SkeletonObject,
                                                  glm::quat,
                                                  RotationSetter>;
  std::unique_ptr<RotationCommand> rotationCommand(
                              new RotationCommand(*skeletonObject,
                                                  &SkeletonObject::setRotation,
                                                  skeletonObject->rotation(),
                                                  skeletonObject->rotation()));

  std::unique_ptr<mtt::CompositeCommand> resultCommand(
                                                  new mtt::CompositeCommand());
  resultCommand->addSubcommand(std::move(positionCommand));
  resultCommand->addSubcommand(std::move(rotationCommand));
  return std::move(resultCommand);
}
