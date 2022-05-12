#include <Objects/ParticleAnimation.h>

ParticleAnimation::ParticleAnimation( const QString& name,
                                      bool canBeRenamed,
                                      const mtt::UID& id) :
  AnimationObject(name, canBeRenamed, id),
  _duration(10 * mtt::second),
  _fieldRef(*this)
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

mtt::TimeRange ParticleAnimation::calculateTiming() const noexcept
{
  return mtt::TimeRange(mtt::TimeT(0), _duration);
}

void ParticleAnimation::update(mtt::TimeRange time)
{
  mtt::TimeRange activeTime = timeRange().intersection(time);
  if(!activeTime.isValid()) return;

  AnimationObject::update(time);

  if(_fieldRef != nullptr) _fieldRef->simulationStep(time);
}
