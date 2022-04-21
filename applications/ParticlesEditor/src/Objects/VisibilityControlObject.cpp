#include <Objects/VisibilityControlObject.h>

VisibilityControlObject::VisibilityControlObject( const QString& name,
                                                  bool canBeRenamed,
                                                  const mtt::UID& id) :
  ModificatorObject(name, canBeRenamed, ParticleField::PREFLUID_TIME, id),
  _saturationRange(0.f, 1.f)
{
}

void VisibilityControlObject::setSaturationRange(
                                    const mtt::Range<float>& newValue) noexcept
{
  if(_saturationRange == newValue) return;
  _saturationRange = newValue;
  emit saturationRangeChanged(newValue);
}

void VisibilityControlObject::simulationStep(mtt::Range<mtt::TimeT> time)
{
  ParticleField* field = fieldRef().get();
  if(field == nullptr) return;

  float startTime = glm::clamp(_saturationRange.min(), 0.f, 1.f);
  float endTime = glm::clamp(_saturationRange.max(), 0.f, 1.f);

  auto theDelegate =
    [&](ParticleField::ParticleData& particle)
    {
      if(particle.maxTime.count() == 0) return;
      float particleTime = float(particle.currentTime.count()) /
                                                float(particle.maxTime.count());
      particleTime = glm::clamp(particleTime, 0.f, 1.f);
      if (particleTime < startTime)
      {
        particle.visibilityFactor =
                                  glm::smoothstep(0.f, startTime, particleTime);
      }
      else if (particleTime > endTime)
      {
        particle.visibilityFactor = glm::smoothstep(1.f, endTime, particleTime);
      }
      else particle.visibilityFactor = 1.f;
    };

  field->updateParticles(theDelegate, typeMask());
}
