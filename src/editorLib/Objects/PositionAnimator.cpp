#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/editorLib/Objects/PositionAnimator.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

PositionAnimator::PositionAnimator( const QString& name,
                                    bool canBeRenamed,
                                    const UID& id) :
  AnimationTrack(name, canBeRenamed, id),
  _targetLink(*this)
{

}

void PositionAnimator::onTimeRangeChanged() noexcept
{
  setTimeRange(KeypointsAnimatedTransform::timeRange());
}

void PositionAnimator::update(TimeRange time)
{
  if(!enabled()) return;
  if(!timeRange().isValid()) return;
  if(!time.isValid()) return;
  if(!timeRange().contains(time.finish())) return;

  ScalableObject* target = targetRef().get();
  if(target == nullptr) return;

  target->setPosition(positionAnimation().value(time.finish()));
  target->setRotation(rotationAnimation().value(time.finish()));
  target->setScale(scaleAnimation().value(time.finish()));
}

std::unique_ptr<AbstractEditCommand>
                                    PositionAnimator::makeRestoreCommand() const
{
  ScalableObject* target = targetRef().get();
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
