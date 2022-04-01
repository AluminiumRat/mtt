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

void HeaterObject::collectCells( Cells& targetCells, float& totalWeight) const
{
  ParticleField* field = fieldRef().get();
  FluidObject& fluid = field->fluid();

  float radius = _size / 2.f;

  glm::mat4 toField = glm::inverse(field->localToWorldTransform()) *
                                                        localToWorldTransform();
  mtt::Box localBound(glm::vec3(-radius), glm::vec3(radius));
  mtt::Box boundInField = localBound.translated(toField);
  if(!boundInField.valid()) return;

  FluidMatrix<float>* temperatureMatrix = fluid.temperatureMatrix();

  glm::vec3 startCorner =
                        glm::round(fluid.toMatrixCoord(boundInField.minCorner));
  startCorner = glm::max(startCorner, glm::vec3(1.f));
  size_t startX = glm::min( size_t(startCorner.x),
                            temperatureMatrix->xSize() - 2);
  size_t startY = glm::min( size_t(startCorner.y),
                            temperatureMatrix->ySize() - 2);
  size_t startZ = glm::min( size_t(startCorner.z),
                            temperatureMatrix->zSize() - 2);

  glm::vec3 endCorner = glm::round(fluid.toMatrixCoord(boundInField.maxCorner));
  endCorner = glm::max(endCorner, glm::vec3(1.f));
  size_t endX = glm::min(size_t(endCorner.x), temperatureMatrix->xSize() - 1);
  size_t endY = glm::min(size_t(endCorner.y), temperatureMatrix->xSize() - 1);
  size_t endZ = glm::min(size_t(endCorner.z), temperatureMatrix->xSize() - 1);

  glm::mat4 fromField = glm::inverse(toField);

  glm::vec3 cellPosition(startX + .5f, startY + .5f, startZ + .5f);
  for (size_t x = startX; x < endX; x++)
  {
    cellPosition.y = startY + .5f;
    for (size_t y = startY; y < endY; y++)
    {
      cellPosition.z = startZ + .5f;
      for (size_t z = startZ; z < endZ; z++)
      {
        glm::vec4 filedCoord = glm::vec4(fluid.toFieldCoord(cellPosition), 1.f);
        glm::vec3 localCoord = fromField * filedCoord;

        float distance = glm::length(localCoord);
        float weight = 1.f - distance / radius;
        if (weight > 0.f)
        {
          targetCells.push_back({x,y,z,weight});
          totalWeight += weight;
        }
        cellPosition.z += 1.f;
      }
      cellPosition.y += 1.f;
    }
    cellPosition.x += 1.f;
  }
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
  collectCells(cells, totalWeight);

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
