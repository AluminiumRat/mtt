#include <Objects/GravityModificator.h>

GravityModificator::GravityModificator( const QString& name,
                                        bool canBeRenamed,
                                        const mtt::UID& id) :
  ModificatorObject(name, canBeRenamed, ParticleField::PREFLUID_TIME, id),
  _acceleration(9.8f)
{
}

void GravityModificator::setAcceleration(float newValue) noexcept
{
  if(_acceleration == newValue) return;
  _acceleration = newValue;
  emit accelerationChanged(newValue);
}

void GravityModificator::simulationStep(mtt::TimeRange time)
{
  ParticleField* field = fieldRef().get();
  if (field == nullptr) return;

  glm::mat4x4 toField = glm::inverse(field->localToWorldTransform()) *
                                                        localToWorldTransform();

  glm::vec3 accelerationVector = toField * glm::vec4(0.f, 0.f, -1.f, 0.f);
  accelerationVector *= _acceleration;

  float floatDeltaTime = mtt::toFloatTime(time.length());
  glm::vec3 deltaV = accelerationVector * floatDeltaTime;

  auto theDelegate =
    [&](ParticleField::ParticleData& particle)
    {
      particle.speed += deltaV;
    };

  field->updateParticles(theDelegate, typeMask());
}
