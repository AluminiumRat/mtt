#include <Objects/ModificatorObject.h>

ModificatorObject::ModificatorObject( const QString& name,
                                      bool canBeRenamed,
                                      const mtt::UID& id) :
  ScalableObject(name, canBeRenamed, id),
  _enabled(true),
  _typeMask(1),
  _fieldRef(*this)
{
}

void ModificatorObject::setEnabled(bool newValue) noexcept
{
  if(_enabled == newValue) return;
  _enabled = newValue;
  emit enabledChanged(newValue);
}

void ModificatorObject::setTypeMask(uint32_t newValue) noexcept
{
  if(_typeMask == newValue) return;
  _typeMask = newValue;
  emit typeMaskChanged(newValue);
}

void ModificatorObject::simulationStep( mtt::TimeT currentTime,
                                        mtt::TimeT delta)
{
}

void ModificatorObject::_connectToField(ParticleField& field)
{
  field.registerModificator(*this);
}

void ModificatorObject::_disconnectFromField(ParticleField& field) noexcept
{
  field.unregisterModificator(*this);
}
