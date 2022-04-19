#include <Objects/HeatingAction.h>

HeatingAction::HeatingAction( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  AnimationAction(name, canBeRenamed, id),
  _emittedEnergy(0),
  _heaterRef(*this)
{
}

void HeatingAction::setEmittedEnergy(float newValue) noexcept
{
  if(newValue < 0.f) newValue = 0.f;
  if(_emittedEnergy == newValue) return;
  _emittedEnergy = newValue;
  emit emittedEnergyChanged(newValue);
}

void HeatingAction::makeAction(float portion)
{
  if(_heaterRef == nullptr) return;
  if(!_heaterRef->enabled()) return;
  if(_emittedEnergy <= 0) return;
  _heaterRef->emitEnergy(_emittedEnergy * portion);
}
