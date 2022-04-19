#include <Objects/ParticlesEmissionAction.h>

ParticlesEmissionAction::ParticlesEmissionAction( const QString& name,
                                                  bool canBeRenamed,
                                                  const mtt::UID& id) :
  AnimationAction(name, canBeRenamed, id),
  _particlesNumber(),
  _emitterRef(*this)
{
}

void ParticlesEmissionAction::setParticlesNumber(uint newValue) noexcept
{
  if(_particlesNumber == newValue) return;
  _particlesNumber = newValue;
  emit particlesNumberChanged(newValue);
}

void ParticlesEmissionAction::makeAction(float portion)
{
  if(_emitterRef == nullptr) return;
  if(!_emitterRef->enabled()) return;

  float particlesNumber = float(_particlesNumber) * portion;

  _emitterRef->emitParticles(particlesNumber);
}
