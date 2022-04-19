#include <mtt/utilities/Box.h>

#include <Objects/Fluid/FluidModificator.h>
#include <Objects/Fluid/FluidObject.h>
#include <Objects/ParticleField.h>

FluidModificator::FluidModificator( const QString& name,
                                    bool canBeRenamed,
                                    const mtt::UID& id) :
  ModificatorObject(name, canBeRenamed, ParticleField::PREFLUID_TIME, id),
  _size(1.f)
{
}

void FluidModificator::setSize(float newValue) noexcept
{
  newValue = glm::max(newValue, 0.f);
  if(_size == newValue) return;
  _size = newValue;
  emit sizeChanged(newValue);
}

FluidModificator::Cells FluidModificator::collectCells() const
{
  Cells cells;
  float totalWeight = 0.f;

  ParticleField* field = fieldRef().get();
  glm::mat4 toField = glm::inverse(field->localToWorldTransform()) *
                                                        localToWorldTransform();

  glm::mat4 fromField = glm::inverse(toField);
  float cellSize = field->fluid().cellSize();
  glm::vec3 cellDiagonal(cellSize, cellSize, cellSize);
  cellDiagonal = fromField * glm::vec4(cellDiagonal, 0.f);
  float cellDiagonalLength = glm::length(cellDiagonal);

  float radius = _size / 2.f;
  float extRadius = radius + cellDiagonalLength / 2.f;

  auto heaterDelegate =
    [&](size_t x, size_t y, size_t z, const glm::vec3& fieldCoord)
    {
      glm::vec3 localCoord = fromField * glm::vec4(fieldCoord, 1.f);

      float distance = glm::length(localCoord);
      float weight = 1.f - distance / extRadius;
      if (weight > 0.f)
      {
        cells.push_back({x,y,z,weight});
        totalWeight += weight;
      }
    };

  mtt::Box localBound(glm::vec3(-radius), glm::vec3(radius));
  mtt::Box boundInField = localBound.translated(toField);
  field->fluid().passCells(boundInField, heaterDelegate);

  for (CellRecord& cell : cells)
  {
    cell.weight /= totalWeight;
  }

  return cells;
}
