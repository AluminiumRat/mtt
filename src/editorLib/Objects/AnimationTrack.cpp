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

void AnimationTrack::update(TimeType time)
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

  using PositionSetter = void (ScalableObject::*)(const glm::vec3&);
  using PositionCommand = SetPropertyCommand< ScalableObject,
                                              glm::vec3,
                                              PositionSetter>;
  std::unique_ptr<PositionCommand> positionCommand(
                              new PositionCommand(*target,
                                                  &ScalableObject::setPosition,
                                                  target->position(),
                                                  target->position()));

  using RotationSetter = void (ScalableObject::*)(const glm::quat&);
  using RotationCommand = SetPropertyCommand< ScalableObject,
                                              glm::quat,
                                              RotationSetter>;
  std::unique_ptr<RotationCommand> rotationCommand(
                              new RotationCommand(*target,
                                                  &ScalableObject::setRotation,
                                                  target->rotation(),
                                                  target->rotation()));

  using ScaleSetter = void (ScalableObject::*)(const glm::vec3&);
  using ScaleCommand = SetPropertyCommand<ScalableObject,
                                          glm::vec3,
                                          ScaleSetter>;
  std::unique_ptr<ScaleCommand> scaleCommand(
                                    new ScaleCommand( *target,
                                                      &ScalableObject::setScale,
                                                      target->scale(),
                                                      target->scale()));

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
