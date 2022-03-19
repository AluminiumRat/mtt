#include <Objects/ParticleAnimation.h>

ParticleAnimation::ParticleAnimation( const QString& name,
                                      bool canBeRenamed,
                                      const mtt::UID& id) :
  AnimationObject(name, canBeRenamed, id),
  _duration(std::chrono::duration_cast<mtt::TimeT>(std::chrono::seconds(10))),
  _fieldRef(*this),
  _lastTime(0)
{
  updateTiming();
}

void ParticleAnimation::setDuration(mtt::TimeT newValue) noexcept
{
  if(newValue.count() < 0) newValue = mtt::TimeT(0);
  if(_duration == newValue) return;
  _duration = newValue;
  updateTiming();
  emit durationChanged(newValue);
}

mtt::Range<mtt::TimeT> ParticleAnimation::calculateTiming() const noexcept
{
  return mtt::Range<mtt::TimeT>(mtt::TimeT(0), _duration);
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
