#include <mtt/utilities/Box.h>

#include <Objects/Fluid/FluidObject.h>
#include <Objects/Fluid/HeaterObject.h>
#include <Objects/ParticleField.h>

HeaterObject::HeaterObject( const QString& name,
                            bool canBeRenamed,
                            const mtt::UID& id) :
  FluidModificator(name, canBeRenamed),
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

void HeaterObject::simulationStep(mtt::TimeT currentTime, mtt::TimeT deltaT)
{
  if(size() <= 0.f) return;
  if(_power <= 0.f) return;

  ParticleField* field = fieldRef().get();
  FluidObject& fluid = field->fluid();

  FluidMatrix<float>* temperatureMatrix = fluid.temperatureMatrix();
  if (temperatureMatrix == nullptr) return;

  using FloatTime = std::chrono::duration<float>;
  float floatDeltaTime = std::chrono::duration_cast<FloatTime>(deltaT).count();
  if(floatDeltaTime <= 0.f) return;

  Cells cells = collectCells();
  for (const CellRecord& cell : cells)
  {
    float cellPower = _power * cell.weight;
    float cellEnergy = cellPower * floatDeltaTime;
    float cellMass = fluid.getCellMass(cell.x, cell.y, cell.z);
    float deltaTemperature = cellEnergy / FluidObject::heatCapacity / cellMass;

    float oldTemperature = temperatureMatrix->get(cell.x, cell.y, cell.z);
    temperatureMatrix->set( cell.x,
                            cell.y,
                            cell.z,
                            oldTemperature + deltaTemperature);
  }
}
