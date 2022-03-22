#include <Objects/Fluid/FluidObject.h>
#include <Objects/ParticleField.h>

FluidObject::FluidObject( const QString& name,
                          bool canBeRenamed,
                          ParticleField& parent,
                          const mtt::UID& id) :
  mtt::Object(name, canBeRenamed, id),
  _parent(parent),
  _typeMask(1),
  _cellSize(1.f)
{
}

void FluidObject::setTypeMask(uint32_t newValue) noexcept
{
  if (_typeMask == newValue) return;
  _typeMask = newValue;
  emit typeMaskChanged(newValue);
}

void FluidObject::setCellSize(float newValue) noexcept
{
  newValue = glm::max(newValue, 0.f);
  if(_cellSize == newValue) return;
  _cellSize = newValue;
  emit cellSizeChanged(newValue);
}

void FluidObject::simulationStep(mtt::TimeT currentTime, mtt::TimeT delta)
{
}
