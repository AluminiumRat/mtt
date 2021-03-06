#include <Objects/Fluid/GasSource.h>

GasSource::GasSource( const QString& name,
                      bool canBeRenamed,
                      const mtt::UID& id) :
  FluidModificator(name, canBeRenamed, id),
  _flowRate(0.f),
  _temperature(FluidObject::defaultTemperature)
{
}

void GasSource::setFlowRate(float newValue) noexcept
{
  newValue = glm::max(newValue, 0.f);
  if(_flowRate == newValue) return;
  _flowRate = newValue;
  emit flowRateChanged(newValue);
}

void GasSource::setTemperature(float newValue) noexcept
{
  newValue = glm::max(newValue, 0.f);
  if(_temperature == newValue) return;
  _temperature = newValue;
  emit temperatureChanged(newValue);
}

void GasSource::emitGas(float volume)
{
  if(!enabled()) return;
  if(size() <= 0.f) return;

  ParticleField* field = fieldRef().get();
  if(field == nullptr) return;
  FluidObject& fluid = field->fluid();

  FluidMatrix<float>* temperatureMatrix = fluid.temperatureMatrix();
  if (temperatureMatrix == nullptr) return;
  FluidMatrix<float>* pressureMatrix = fluid.pressureMatrix();
  if (pressureMatrix == nullptr) return;

  float massIncrement = fluid.getGasMass( volume,
                                          FluidObject::defaultPressure,
                                          _temperature);
  float energyIncrement =
                      massIncrement * _temperature * FluidObject::heatCapacity;

  Cells cells = collectCells();
  for (const CellRecord& cell : cells)
  {
    float cellAdditionVolume = volume * cell.weight;
    float cellAdditionalMass = massIncrement * cell.weight;
    float cellAdditionalEnergy = energyIncrement * cell.weight;

    float cellMass = fluid.getCellMass(cell.x, cell.y, cell.z);
    float cellTemperature = temperatureMatrix->get(cell.x, cell.y, cell.z);
    float cellEnergy = cellMass * cellTemperature * FluidObject::heatCapacity;
    float newCellEnergy = cellEnergy + cellAdditionalEnergy;
    float newCellMass = cellMass + cellAdditionalMass;
    float newCellTemperature =
                        newCellEnergy / FluidObject::heatCapacity / newCellMass;
    temperatureMatrix->set(cell.x, cell.y, cell.z, newCellTemperature);

    float additionalPressure =
        cellAdditionVolume / fluid.cellVolume() * FluidObject::defaultPressure;
    float newPressure =
              additionalPressure + pressureMatrix->get(cell.x, cell.y, cell.z);
    pressureMatrix->set(cell.x, cell.y, cell.z, newPressure);
  }
}

void GasSource::simulationStep(mtt::TimeRange time)
{
  if(_flowRate <= 0.f) return;

  float floatDeltaTime = mtt::toFloatTime(time.length());
  if (floatDeltaTime <= 0.f) return;

  float volumeIncrement = _flowRate * floatDeltaTime;

  emitGas(volumeIncrement);
}
