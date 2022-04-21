#include <mtt/utilities/Box.h>

#include <Objects/Fluid/FluidObject.h>
#include <Objects/Fluid/HeaterObject.h>
#include <Objects/ParticleField.h>

HeaterObject::HeaterObject( const QString& name,
                            bool canBeRenamed,
                            const mtt::UID& id) :
  FluidModificator(name, canBeRenamed, id),
  _power(1000.f)
{
}

void HeaterObject::setPower(float newValue) noexcept
{
  newValue = glm::max(newValue, 0.f);
  if(_power == newValue) return;
  _power = newValue;
  emit powerChanged(newValue);
}

void HeaterObject::emitEnergy(float energy)
{
  if (!enabled()) return;
  if (size() <= 0.f) return;

  ParticleField* field = fieldRef().get();
  if(field == nullptr) return;
  FluidObject& fluid = field->fluid();

  FluidMatrix<float>* temperatureMatrix = fluid.temperatureMatrix();
  if (temperatureMatrix == nullptr) return;

  Cells cells = collectCells();
  for (const CellRecord& cell : cells)
  {
    float cellEnergy = energy * cell.weight;
    float cellMass = fluid.getCellMass(cell.x, cell.y, cell.z);
    float deltaTemperature = cellEnergy / FluidObject::heatCapacity / cellMass;

    float oldTemperature = temperatureMatrix->get(cell.x, cell.y, cell.z);
    temperatureMatrix->set( cell.x,
                            cell.y,
                            cell.z,
                            oldTemperature + deltaTemperature);
  }
}

void HeaterObject::simulationStep(mtt::TimeRange time)
{
  if(_power <= 0.f) return;

  float floatDeltaTime = mtt::toFloatTime(time.length());
  if(floatDeltaTime <= 0.f) return;

  float energy = _power * floatDeltaTime;
  emitEnergy(energy);
}
