#include <Objects/GasEmissionAction.h>

GasEmissionAction::GasEmissionAction( const QString& name,
                                      bool canBeRenamed,
                                      const mtt::UID& id) :
  AnimationAction(name, canBeRenamed, id),
  _gasSourceRef(*this),
  _emittedVolume(0.f)
{
}

void GasEmissionAction::setEmittedVolume(float newValue) noexcept
{
  if(newValue < 0.f) newValue = 0.f;
  if(_emittedVolume == newValue) return;
  _emittedVolume = newValue;
  emit emittedVolumeChanged(newValue);
}

void GasEmissionAction::makeAction(float portion)
{
}
