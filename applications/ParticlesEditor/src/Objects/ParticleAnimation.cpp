#include <Objects/ParticleAnimation.h>

ParticleAnimation::ParticleAnimation( const QString& name,
                                      bool canBeRenamed,
                                      const mtt::UID& id) :
  AnimationObject(name, canBeRenamed, id),
  _fieldRef(*this)
{
}

void ParticleAnimation::setTimeRange(
                                  const mtt::Range<TimeType>& newValue) noexcept
{
  _storedTimeRange = newValue;
  updateTiming();
}

mtt::Range<ParticleAnimation::TimeType>
                              ParticleAnimation::calculateTiming() const noexcept
{
  return _storedTimeRange;
}

void ParticleAnimation::update(TimeType time)
{
  if(time < startTime() || time > finishTime()) return;

  AnimationObject::update(time);

  if(_lastTime > time) _lastTime = time;
  if (_fieldRef != nullptr)
  {
    _fieldRef->simulationStep(time, time - _lastTime);
  }
  _lastTime = time;
}
