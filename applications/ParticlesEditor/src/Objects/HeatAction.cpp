#include <Objects/HeatAction.h>

HeatAction::HeatAction( const QString& name,
                        bool canBeRenamed,
                        const mtt::UID& id) :
  ActionAnimationTrack(name, canBeRenamed, id),
  _emittedEnergy(0),
  _heaterRef(*this)
{
}

void HeatAction::setEmittedEnergy(float newValue) noexcept
{
  if(newValue < 0.f) newValue = 0.f;
  if(_emittedEnergy == newValue) return;
  _emittedEnergy = newValue;
  emit emittedEnergyChanged(newValue);
}

void HeatAction::makeAction(float portion)
{
}
