#include <Objects/SizeControlObject.h>

SizeControlObject::SizeControlObject( const QString& name,
                                      bool canBeRenamed,
                                      const mtt::UID& id) :
  ModificatorObject(name, canBeRenamed, ParticleField::PREFLUID_TIME, id),
  _startSize(0.f),
  _endSize(1.f)
{
}

void SizeControlObject::setStartSize(float newValue) noexcept
{
  newValue = glm::max(0.f, newValue);
  if(_startSize == newValue) return;
  _startSize = newValue;
  emit startSizeChanged(newValue);
}

void SizeControlObject::setEndSize(float newValue) noexcept
{
  newValue = glm::max(0.f, newValue);
  if(_endSize == newValue) return;
  _endSize = newValue;
  emit endSizeChanged(newValue);
}

void SizeControlObject::simulationStep(mtt::Range<mtt::TimeT> time)
{
  ParticleField* field = fieldRef().get();
  if (field == nullptr) return;

  auto theDelegate =
    [&](ParticleField::ParticleData& particle)
    {
      if(particle.maxTime.count() == 0) return;
      float particleTime = float(particle.currentTime.count()) /
                                                float(particle.maxTime.count());
      particleTime = glm::clamp(particleTime, 0.f, 1.f);
      particle.size = glm::mix(_startSize, _endSize, particleTime);
    };

  field->updateParticles(theDelegate, typeMask());
}
