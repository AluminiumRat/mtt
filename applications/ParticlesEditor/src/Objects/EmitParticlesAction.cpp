#include <Objects/EmitParticlesAction.h>

EmitParticlesAction::EmitParticlesAction( const QString& name,
                                          bool canBeRenamed,
                                          const mtt::UID& id) :
  ActionAnimationTrack(name, canBeRenamed, id),
  _particlesNumber(),
  _emitterRef(*this)
{
}

void EmitParticlesAction::setParticlesNumber(uint newValue) noexcept
{
  if(_particlesNumber == newValue) return;
  _particlesNumber = newValue;
  emit particlesNumberChanged(newValue);
}

void EmitParticlesAction::makeAction(float portion)
{
  if(_emitterRef == nullptr) return;
  if(!_emitterRef->enabled()) return;

  float particlesNumber = float(_particlesNumber) * portion;

  _emitterRef->emitParticles(particlesNumber);
}
