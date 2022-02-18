#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

AnimationTrack::AnimationTrack( const QString& name,
                                bool canBeRenamed,
                                const mtt::UID& id) :
  Object(name, canBeRenamed,id),
  _enabled(true),
  _skeletonLink(*this)
{
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

  mtt::SkeletonObject* skeletonObject = skeleton();
  if(skeletonObject == nullptr) return;

  skeletonObject->setPosition(positionAnimation().value(time));
  skeletonObject->setRotation(rotationAnimation().value(time));
  skeletonObject->setScale(scaleAnimation().value(time));
}

void AnimationTrack::setSkeleton(mtt::SkeletonObject* skeleton)
{
  if(skeleton == nullptr) setSkeletonId(mtt::UID());
  else setSkeletonId(skeleton->id());
}

void AnimationTrack::setSkeletonId(const mtt::UID& id)
{
  try
  {
    _skeletonLink.setReferencedId(id);
  }
  catch(...)
  {
    try
    {
      _skeletonLink.setReferencedId(mtt::UID());
    }
    catch(...)
    {
      mtt::Abort("AnimationTrack::setSkeletonId: unable to emplace skeleton link");
    }
    throw;
  }
}

void AnimationTrack::_connectSkeleton(mtt::SkeletonObject& skeleton)
{
  emit skeletonRefChanged(&skeleton);
}

void AnimationTrack::_disconnectSkeleton(mtt::SkeletonObject& skeleton) noexcept
{
  emit skeletonRefChanged(nullptr);
}

std::unique_ptr<mtt::AbstractEditCommand>
                                    AnimationTrack::makeRestoreCommand() const
{
  mtt::SkeletonObject* skeletonObject = skeleton();
  if(skeletonObject == nullptr) return nullptr;

  using PositionSetter = void (mtt::SkeletonObject::*)(const glm::vec3&);
  using PositionCommand = mtt::SetPropertyCommand<mtt::SkeletonObject,
                                                  glm::vec3,
                                                  PositionSetter>;
  std::unique_ptr<PositionCommand> positionCommand(
                          new PositionCommand(*skeletonObject,
                                              &mtt::SkeletonObject::setPosition,
                                              skeletonObject->position(),
                                              skeletonObject->position()));

  using RotationSetter = void (mtt::SkeletonObject::*)(const glm::quat&);
  using RotationCommand = mtt::SetPropertyCommand<mtt::SkeletonObject,
                                                  glm::quat,
                                                  RotationSetter>;
  std::unique_ptr<RotationCommand> rotationCommand(
                          new RotationCommand(*skeletonObject,
                                              &mtt::SkeletonObject::setRotation,
                                              skeletonObject->rotation(),
                                              skeletonObject->rotation()));

  using ScaleSetter = void (mtt::SkeletonObject::*)(const glm::vec3&);
  using ScaleCommand = mtt::SetPropertyCommand< mtt::SkeletonObject,
                                                glm::vec3,
                                                ScaleSetter>;
  std::unique_ptr<ScaleCommand> scaleCommand(
                              new ScaleCommand( *skeletonObject,
                                                &mtt::SkeletonObject::setScale,
                                                skeletonObject->scale(),
                                                skeletonObject->scale()));

  std::unique_ptr<mtt::CompositeCommand> resultCommand(
                                                  new mtt::CompositeCommand());
  resultCommand->addSubcommand(std::move(positionCommand));
  resultCommand->addSubcommand(std::move(rotationCommand));
  resultCommand->addSubcommand(std::move(scaleCommand));
  return std::move(resultCommand);
}

void AnimationTrack::onStartTimeChanged() noexcept
{
  emit startTimeChanged(startTime());
}

void AnimationTrack::onFinishTimeChanged() noexcept
{
  emit finishTimeChanged(finishTime());
}

void AnimationTrack::onTimingChanged() noexcept
{
  emit timingChanged();
}

void AnimationTrack::onDurationChanged() noexcept
{
  emit durationChanged(duration());
}
