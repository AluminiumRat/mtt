#include <mtt/utilities/Box.h>

#include <Objects/Fluid/FluidObject.h>
#include <Objects/HeaterObject.h>
#include <Objects/ParticleField.h>

HeaterObject::HeaterObject( const QString& name,
                            bool canBeRenamed,
                            const mtt::UID& id) :
  ModificatorObject(name, canBeRenamed, ParticleField::PREFLUID_TIME),
  _size(1.f),
  _power(1000.f)
{
}

void HeaterObject::setSize(float newValue) noexcept
{
  newValue = glm::max(newValue, 0.f);
  if(_size == newValue) return;
  _size = newValue;
  emit sizeChanged(newValue);
}

void HeaterObject::setPower(float newValue) noexcept
{
  newValue = glm::max(newValue, 0.f);
  if(_power == newValue) return;
  _power = newValue;
  emit powerChanged(newValue);
}

void HeaterObject::_collectCells( Cells& targetCells, float& totalWeight) const
{
  ParticleField* field = fieldRef().get();
  glm::mat4 toField = glm::inverse(field->localToWorldTransform()) *
                                                        localToWorldTransform();

  float radius = _size / 2.f;
  mtt::Box localBound(glm::vec3(-radius), glm::vec3(radius));
  mtt::Box boundInField = localBound.translated(toField);

  glm::mat4 fromField = glm::inverse(toField);

  auto heaterDelegate =
    [&](size_t x, size_t y, size_t z, const glm::vec3& fieldCoord)
    {
      glm::vec3 localCoord = fromField * glm::vec4(fieldCoord, 1.f);

      float distance = glm::length(localCoord);
      float weight = 1.f - distance / radius;
      if (weight > 0.f)
      {
        targetCells.push_back({x,y,z,weight});
        totalWeight += weight;
      }
    };

  field->fluid().passCells(boundInField, heaterDelegate);
}

void HeaterObject::simulationStep(mtt::TimeT currentTime, mtt::TimeT deltaT)
{
  if(_size <= 0.f) return;
  if(_power <= 0.f) return;

  ParticleField* field = fieldRef().get();
  FluidObject& fluid = field->fluid();

  FluidMatrix<float>* temperatureMatrix = fluid.temperatureMatrix();
  if (temperatureMatrix == nullptr) return;

  Cells cells;
  float totalWeight = 0.f;
  _collectCells(cells, totalWeight);

  using FloatTime = std::chrono::duration<float>;
  float floatDeltaTime = std::chrono::duration_cast<FloatTime>(deltaT).count();

  constexpr float heatCapacity = 1000.f;

  for (const CellRecord& cell : cells)
  {
    float weight = cell.weight / totalWeight;
    float cellPower = _power * weight;
    float cellEnergy = cellPower * floatDeltaTime;
    float cellMass = fluid.getCellMass(cell.x, cell.y, cell.z);
    float deltaTemperature = cellEnergy / heatCapacity / cellMass;

    float oldTemperature = temperatureMatrix->get(cell.x, cell.y, cell.z);
    temperatureMatrix->set( cell.x,
                            cell.y,
                            cell.z,
                            oldTemperature + deltaTemperature);
  }
}
