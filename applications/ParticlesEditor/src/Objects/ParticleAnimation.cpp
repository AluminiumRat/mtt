#include <Objects/ParticleAnimation.h>

ParticleAnimation::ParticleAnimation( const QString& name,
                                      bool canBeRenamed,
                                      const mtt::UID& id) :
  AnimationObject(name, canBeRenamed, id),
  _fieldRef(*this),
  _lastTime(0)
{
}

void ParticleAnimation::setTimeRange(
                                const mtt::Range<mtt::TimeT>& newValue) noexcept
{
  _storedTimeRange = newValue;
  updateTiming();
}

mtt::Range<mtt::TimeT> ParticleAnimation::calculateTiming() const noexcept
{
  return _storedTimeRange;
}

void ParticleAnimation::update(mtt::TimeT time)
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
