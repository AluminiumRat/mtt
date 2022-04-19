#include <Objects/EmitGasAction.h>

EmitGasAction::EmitGasAction( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  ActionAnimationTrack(name, canBeRenamed, id),
  _gasSourceRef(*this),
  _emittedVolume(0.f)
{
}

void EmitGasAction::setEmittedVolume(float newValue) noexcept
{
  if(newValue < 0.f) newValue = 0.f;
  if(_emittedVolume == newValue) return;
  _emittedVolume = newValue;
  emit emittedVolumeChanged(newValue);
}

void EmitGasAction::makeAction(float portion)
{
}
