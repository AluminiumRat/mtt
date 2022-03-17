#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

AnimationTrack::AnimationTrack( const QString& name,
                                bool canBeRenamed,
                                const UID& id) :
  Object(name, canBeRenamed,id),
  _enabled(true),
  _targetLink(*this)
{
}

void AnimationTrack::setEnabled(bool newValue)
{
  if(newValue == _enabled) return;
  _enabled = newValue;
  emit enabledChanged(_enabled);
}

void AnimationTrack::update(TimeT time)
{
  if(!enabled()) return;
  if(time < startTime() || time > finishTime()) return;

  ScalableObject* target = _targetLink.get();
  if(target == nullptr) return;

  target->setPosition(positionAnimation().value(time));
  target->setRotation(rotationAnimation().value(time));
  target->setScale(scaleAnimation().value(time));
}

std::unique_ptr<AbstractEditCommand> AnimationTrack::makeRestoreCommand() const
{
  ScalableObject* target = _targetLink.get();
  if(target == nullptr) return nullptr;

  std::unique_ptr<AbstractEditCommand> positionCommand =
                          makeSetPropertyCommand( *target,
                                                  &ScalableObject::position,
                                                  &ScalableObject::setPosition,
                                                  target->position());

  std::unique_ptr<AbstractEditCommand> rotationCommand =
                          makeSetPropertyCommand( *target,
                                                  &ScalableObject::rotation,
                                                  &ScalableObject::setRotation,
                                                  target->rotation());

  std::unique_ptr<AbstractEditCommand> scaleCommand =
                              makeSetPropertyCommand( *target,
                                                      &ScalableObject::scale,
                                                      &ScalableObject::setScale,
                                                      target->scale());

  std::unique_ptr<CompositeCommand> resultCommand(new CompositeCommand());
  resultCommand->addSubcommand(std::move(positionCommand));
  resultCommand->addSubcommand(std::move(rotationCommand));
  resultCommand->addSubcommand(std::move(scaleCommand));
  return std::move(resultCommand);
}

void AnimationTrack::onTimeRangeChanged() noexcept
{
  emit timeRangeChanged(timeRange());
}
