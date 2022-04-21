#include <Objects/ModificatorObject.h>

ModificatorObject::ModificatorObject(
                                  const QString& name,
                                  bool canBeRenamed,
                                  ParticleField::ModificatorApplyTime applyTime,
                                  const mtt::UID& id) :
  HierarhicalObject(name, canBeRenamed, id),
  _applyTime(applyTime),
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

void ModificatorObject::simulationStep(mtt::TimeRange time)
{
}

void ModificatorObject::connectToField(ParticleField& field)
{
  field.registerModificator(*this);
}

void ModificatorObject::disconnectFromField(ParticleField& field) noexcept
{
  field.unregisterModificator(*this);
}
