#include <Objects/Fluid/FluidObject.h>
#include <Objects/ParticleField.h>

#define PROJECT_ITERATIONS 100

FluidObject::FluidObject( const QString& name,
                          bool canBeRenamed,
                          ParticleField& parent,
                          const mtt::UID& id) :
  mtt::Object(name, canBeRenamed, id),
  _parent(parent),
  _typeMask(1),
  _cellSize(1.f),
  _wind(0.f)
{
  connect(&parent,
          &ParticleField::sizeChanged,
          this,
          &FluidObject::_resetMatrices,
          Qt::DirectConnection);
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
  _resetMatrices();
  emit cellSizeChanged(newValue);
}

void FluidObject::setWind(const glm::vec3& newValue) noexcept
{
  if(_wind == newValue) return;
  _wind = newValue;
  _resetMatrices();
  emit windChanged(newValue);
}

void FluidObject::clear() noexcept
{
  _resetMatrices();
}

void FluidObject::_resetMatrices() noexcept
{
  _velocityMatrix.reset();
}

void FluidObject::_rebuildMatrices()
{
  glm::vec3 fieldSize = _parent.size();
  if(fieldSize.x <= 0.f || fieldSize.y <= 0.f || fieldSize.z <= 0.f) return;

  glm::vec3 fieldExtent = glm::ceil(fieldSize / _cellSize);
  size_t fieldExtentX = size_t(fieldExtent.x) + 2;
  size_t fieldExtentY = size_t(fieldExtent.y) + 2;
  size_t fieldExtentZ = size_t(fieldExtent.z) + 2;

  _velocityMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
  _velocityMatrix->clear(_wind);
}

void FluidObject::simulationStep(mtt::TimeT currentTime, mtt::TimeT delta)
{
  if(!_velocityMatrix.has_value()) _rebuildMatrices();
  if(!_velocityMatrix.has_value()) return;

  float dTime =
        std::chrono::duration_cast<std::chrono::duration<float>>(delta).count();

  _velocityMatrix->set( _velocityMatrix->xSize() / 2,
                        _velocityMatrix->ySize() / 2,
                        _velocityMatrix->zSize() / 2,
                        glm::vec3(5, 0, 0));

  _projectVelocity();

  _updateParticles(dTime);
}

void FluidObject::_buildDivirgence(FluidMatrix<float>& target)
{
  for (size_t x = 0; x < _velocityMatrix->xSize(); x++)
  {
    for (size_t y = 0; y < _velocityMatrix->ySize(); y++)
    {
      for (size_t z = 0; z < _velocityMatrix->zSize(); z++)
      {
        if( x == 0 ||
            x == _velocityMatrix->xSize() - 1 ||
            y == 0 ||
            y == _velocityMatrix->ySize() - 1 ||
            z == 0 ||
            z == _velocityMatrix->zSize() - 1)
        {
          target(x, y, z) = 0;
        }
        else
        {
          float xDivirgence = _velocityMatrix->get(x + 1, y, z).x -
                                            _velocityMatrix->get(x - 1, y, z).x;
          float yDivirgence = _velocityMatrix->get(x, y + 1, z).y -
                                            _velocityMatrix->get(x, y - 1, z).y;
          float zDivirgence = _velocityMatrix->get(x, y, z + 1).z -
                                            _velocityMatrix->get(x, y, z - 1).z;
          target(x, y, z) = xDivirgence + yDivirgence + zDivirgence;
        }
      }
    }
  }
}

void FluidObject::_buildProjPressure( FluidMatrix<float>& target,
                                      const FluidMatrix<float>& divirgence)
{
  FluidMatrix<float> prew(target.xSize(), target.ySize(), target.zSize());
  prew.clear(0.f);
  FluidMatrix<float> next(target.xSize(), target.ySize(), target.zSize());

  for(int iteration = 0;
      iteration < PROJECT_ITERATIONS;
      iteration++)
  {
    for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
    {
      for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
      {
        for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
        {
          float pressure = prew(x - 1, y, z) +
                            prew(x + 1, y, z) +
                            prew(x, y - 1, z) +
                            prew(x, y + 1, z) +
                            prew(x, y, z - 1) +
                            prew(x, y, z + 1) -
                            divirgence(x, y, z);
          next(x, y, z) = pressure / 6;
        }
      }
    }
    prew.swap(next);
  }

  target.swap(prew);
}

void FluidObject::_projectVelocity()
{
  FluidMatrix<float> divirgence(_velocityMatrix->xSize(),
                                _velocityMatrix->ySize(),
                                _velocityMatrix->zSize());
  _buildDivirgence(divirgence);
  FluidMatrix<float> projPressure(_velocityMatrix->xSize(),
                                  _velocityMatrix->ySize(),
                                  _velocityMatrix->zSize());
  _buildProjPressure(projPressure, divirgence);

  for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
      {
        glm::vec3 deltaV(
                        projPressure(x - 1, y, z) - projPressure(x + 1, y, z),
                        projPressure(x, y - 1, z) - projPressure(x, y + 1, z),
                        projPressure(x, y, z - 1) - projPressure(x, y, z + 1));
        deltaV /= 2.f;
        (*_velocityMatrix)(x, y, z) += deltaV;
      }
    }
  }
}

glm::vec3 FluidObject::_toMatrixCoord(
                                    const glm::vec3& fieldCoord) const noexcept
{
  glm::vec3 matrixCoord = fieldCoord + _parent.size() / 2.f;
  matrixCoord /= _cellSize;
  matrixCoord += 1.f;
  return matrixCoord;
}

void FluidObject::_updateParticles(float dTime)
{
  if(dTime <= 0.f) return;

  auto theDelegate =
    [&](ParticleField::ParticleData& particle)
    {
      if(particle.frictionFactor <= 0.f) return;

      glm::vec3 matrixCoord = _toMatrixCoord(particle.position);
      if( matrixCoord.x < 0 ||
          matrixCoord.x >= _velocityMatrix->xSize() - 1 ||
          matrixCoord.y < 0 ||
          matrixCoord.y >= _velocityMatrix->ySize() - 1 || 
          matrixCoord.z < 0 ||
          matrixCoord.z >= _velocityMatrix->zSize() - 1) return;

      glm::vec3 flowSpeed = _velocityMatrix->interpolate(matrixCoord);

      if(particle.mass <= 0.f) particle.speed = flowSpeed;
      else
      {
        float rate = -dTime * particle.frictionFactor / particle.mass;
        glm::vec3 deltaV = flowSpeed - particle.speed;
        particle.speed = flowSpeed - deltaV * exp(rate);
      }
  };

  _parent.updateParticles(theDelegate, _typeMask);
}
