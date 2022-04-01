#include <algorithm>

#include <mtt/utilities/Abort.h>
#include <mtt/utilities/Box.h>

#include <Objects/Fluid/FluidObject.h>
#include <Objects/BlockerObject.h>
#include <Objects/ParticleField.h>

#define PROJECT_ITERATIONS 30

constexpr float defaultTemperature = 273.15f;
constexpr float defaultPressure = 100000.f;
constexpr float frictionFactor = .02f;

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

void FluidObject::registerBlocker(BlockerObject& blocker)
{
  if(std::find( _blockers.begin(),
                _blockers.end(),
                &blocker) != _blockers.end())
  {
    mtt::Abort("FluidObject::registerBlocker: blocker is already registered.");
  }

  try
  {
    _blockers.push_back(&blocker);

    connect(&blocker,
            &BlockerObject::enabledChanged,
            this,
            &FluidObject::_resetBlockMatrix,
            Qt::DirectConnection);

    connect(&blocker,
            &BlockerObject::transformChanged,
            this,
            &FluidObject::_resetBlockMatrix,
            Qt::DirectConnection);

    connect(&blocker,
            &BlockerObject::shapeChanged,
            this,
            &FluidObject::_resetBlockMatrix,
            Qt::DirectConnection);

    connect(&blocker,
            &BlockerObject::sizeChanged,
            this,
            &FluidObject::_resetBlockMatrix,
            Qt::DirectConnection);

    _resetBlockMatrix();
  }
  catch(...)
  {
    unregisterBlocker(blocker);
    throw;
  }
}

void FluidObject::unregisterBlocker(BlockerObject& blocker) noexcept
{
  Blockers::iterator iBlocker = std::find(_blockers.begin(),
                                          _blockers.end(),
                                          &blocker);
  if(iBlocker == _blockers.end()) return;

  _resetBlockMatrix();

  _blockers.erase(iBlocker);
}

void FluidObject::_resetBlockMatrix() noexcept
{
  _blockMatrix.reset();
}

void FluidObject::_resetMatrices() noexcept
{
  _velocityMatrix.reset();
  _temperatureMatrix.reset();
  _pressureMatrix.reset();
  _resetBlockMatrix();
}

void FluidObject::clear() noexcept
{
  _resetMatrices();
}

void FluidObject::_rebuildMatrices()
{
  glm::vec3 fieldSize = _parent.size();
  if(fieldSize.x <= 0.f || fieldSize.y <= 0.f || fieldSize.z <= 0.f) return;

  glm::vec3 fieldExtent = glm::ceil(fieldSize / _cellSize);
  size_t fieldExtentX = size_t(fieldExtent.x) + 3;
  size_t fieldExtentY = size_t(fieldExtent.y) + 3;
  size_t fieldExtentZ = size_t(fieldExtent.z) + 3;

  try
  {
    _velocityMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _velocityMatrix->clear(_wind);

    _temperatureMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _temperatureMatrix->clear(defaultTemperature);

    _pressureMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _pressureMatrix->clear(defaultPressure);

    _massMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _massMatrix->clear(0.f);
  }
  catch(...)
  {
    _resetMatrices();
    throw;
  }
}

void FluidObject::_applyBlocker(BlockerObject& blocker)
{
  if(!blocker.enabled()) return;

  glm::mat4 toBlocker = glm::inverse(blocker.localToWorldTransform()) *
                                                _parent.localToWorldTransform();
  glm::mat4 fromBlocker = glm::inverse(toBlocker);

  float halfSize = blocker.size() / 2.f;
  const glm::vec4 corners[8] =
                            { glm::vec4(-halfSize, -halfSize, -halfSize, 1.f),
                              glm::vec4( halfSize, -halfSize, -halfSize, 1.f),
                              glm::vec4(-halfSize,  halfSize, -halfSize, 1.f),
                              glm::vec4( halfSize,  halfSize, -halfSize, 1.f),
                              glm::vec4(-halfSize, -halfSize,  halfSize, 1.f),
                              glm::vec4( halfSize, -halfSize,  halfSize, 1.f),
                              glm::vec4(-halfSize,  halfSize,  halfSize, 1.f),
                              glm::vec4( halfSize,  halfSize,  halfSize, 1.f)};
  mtt::Box blockerBox;
  for (const glm::vec4& corner : corners)
  {
    blockerBox.extend(fromBlocker * corner);
  }
  if(!blockerBox.valid()) return;

  glm::vec3 startCorner = glm::round(_toMatrixCoord(blockerBox.minCorner));
  startCorner = glm::max(startCorner, glm::vec3(1.f));
  size_t startX = glm::min(size_t(startCorner.x), _blockMatrix->xSize() - 2);
  size_t startY = glm::min(size_t(startCorner.y), _blockMatrix->ySize() - 2);
  size_t startZ = glm::min(size_t(startCorner.z), _blockMatrix->zSize() - 2);

  glm::vec3 endCorner = glm::round(_toMatrixCoord(blockerBox.maxCorner));
  endCorner = glm::max(endCorner, glm::vec3(1.f));
  size_t endX = glm::min(size_t(endCorner.x), _blockMatrix->xSize() - 1);
  size_t endY = glm::min(size_t(endCorner.y), _blockMatrix->ySize() - 1);
  size_t endZ = glm::min(size_t(endCorner.z), _blockMatrix->zSize() - 1);

  glm::vec3 cellPosition(startX + .5f, startY + .5f, startZ + .5f);
  for (size_t x = startX; x < endX; x++)
  {
    cellPosition.y = startY + .5f;
    for (size_t y = startY; y < endY; y++)
    {
      cellPosition.z = startZ + .5f;
      for (size_t z = startZ; z < endZ; z++)
      {
        glm::vec4 filedCoord = glm::vec4(_toFieldCoord(cellPosition), 1.f);
        glm::vec3 blockerCoord = toBlocker * filedCoord;
        if(blocker.isPointInside(blockerCoord)) _blockMatrix->set(x, y, z, 1);
        cellPosition.z += 1.f;
      }
      cellPosition.y += 1.f;
    }
    cellPosition.x += 1.f;
  }
}

void FluidObject::_rebuildBlockMatrix()
{
  try
  {
    _blockMatrix.emplace( _velocityMatrix->xSize(),
                          _velocityMatrix->ySize(),
                          _velocityMatrix->zSize());
    _blockMatrix->clear(0);

    for(BlockerObject* blocker : _blockers) _applyBlocker(*blocker);
  }
  catch (...)
  {
    _resetBlockMatrix();
    throw;
  }
}

void FluidObject::simulationStep(mtt::TimeT currentTime, mtt::TimeT delta)
{
  if(!_velocityMatrix.has_value()) _rebuildMatrices();
  if(!_velocityMatrix.has_value()) return;

  if(!_blockMatrix.has_value()) _rebuildBlockMatrix();

  _temperatureMatrix->set(_temperatureMatrix->xSize() / 2,
                          _temperatureMatrix->ySize() / 2,
                          _temperatureMatrix->zSize() / 2,
                          300.f);

  float dTime =
        std::chrono::duration_cast<std::chrono::duration<float>>(delta).count();

  _calculateMassMatrix();
  _applyArchimedesForce(dTime);
  _applyFriction(dTime),
  _blockVelocity();
  _projectVelocity();
  _moveMatrices(dTime);
  _projectVelocity();

  _updateParticles(dTime);
}

void FluidObject::_calculateMassMatrix() noexcept
{
  float cellVolume = _cellSize * _cellSize * _cellSize;
  constexpr float gasConstant = 287.058f;

  for (size_t x = 0; x < _velocityMatrix->xSize(); x++)
  {
    for (size_t y = 0; y < _velocityMatrix->ySize(); y++)
    {
      for (size_t z = 0; z < _velocityMatrix->zSize(); z++)
      {
        float temperature = _temperatureMatrix->get(x, y, z);
        float pressure = _pressureMatrix->get(x, y, z);
        float density = pressure / (gasConstant * temperature);
        float mass = cellVolume * density;
        _massMatrix->set(x, y, z, mass);
      }
    }
  }
}

void FluidObject::_applyArchimedesForce(float dTime) noexcept
{
  for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
      {
        if(_blockMatrix->get(x, y, z) != 0) continue;

        float neighbourMass = 0.f;
        float neighbourCount = 0.f;
        if (_blockMatrix->get(x + 1, y, z) == 0)
        {
          neighbourMass += _massMatrix->get(x + 1, y, z);
          neighbourCount += 1;
        }
        if (_blockMatrix->get(x - 1, y, z) == 0)
        {
          neighbourMass += _massMatrix->get(x - 1, y, z);
          neighbourCount += 1;
        }
        if (_blockMatrix->get(x, y + 1, z) == 0)
        {
          neighbourMass += _massMatrix->get(x, y + 1, z);
          neighbourCount += 1;
        }
        if (_blockMatrix->get(x, y - 1, z) == 0)
        {
          neighbourMass += _massMatrix->get(x, y - 1, z);
          neighbourCount += 1;
        }

        if(neighbourCount == 0.f) continue;

        float currentMass = _massMatrix->get(x, y, z);
        float force = neighbourMass / neighbourCount - currentMass;
        force *= 9.8f;
        float acceleration = force / currentMass;
        glm::vec3 dVelocity = glm::vec3(0.f, 0.f, 1.f) * acceleration * dTime;
        const glm::vec3& currentVelocity = _velocityMatrix->get(x, y, z);
        _velocityMatrix->set(x, y, z, currentVelocity + dVelocity);
      }
    }
  }
}

template<int direction>
float FluidObject::_getVelocity(size_t x, size_t y, size_t z)
{
  if (_blockMatrix->get(x, y, z) != 0) return 0;
  return _velocityMatrix->get(x, y, z)[direction];
}

template<int direction, int normal1, int normal2>
glm::vec3 FluidObject::_getFrictionForce(size_t x, size_t y, size_t z)
{
  float velocity = 0;

  size_t coords[3] = { x, y, z };
  coords[normal1] += 1;
  velocity += _getVelocity<direction>(coords[0], coords[1], coords[2]);
  coords[normal1] -= 2;
  velocity += _getVelocity<direction>(coords[0], coords[1], coords[2]);

  coords[normal1] += 2;
  coords[normal2] += 1;
  velocity += _getVelocity<direction>(coords[0], coords[1], coords[2]);
  coords[normal2] -= 2;
  velocity += _getVelocity<direction>(coords[0], coords[1], coords[2]);

  velocity /= 4.f;

  velocity -= _getVelocity<direction>(x, y, z);

  /*if (abs(velocity) > 1)
  {
    mtt::Log() << velocity;
  }*/

  glm::vec3 force(0.f);
  force[direction] = velocity * frictionFactor * _cellSize;
                //velocity * 8.f * glm::pi<float>() * frictionFactor * _cellSize;

  return force;
}

void FluidObject::_applyFriction(float dTime) noexcept
{
  for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
      {
        if (_blockMatrix->get(x, y, z) != 0) continue;
        glm::vec3 force = _getFrictionForce<0, 1, 2>(x, y, z) +
                          _getFrictionForce<1, 0, 2>(x, y, z) +
                          _getFrictionForce<2, 0, 1>(x, y, z);

        float mass = _massMatrix->get(x, y, z);
        glm::vec3 dVelocity = force / mass * dTime;
        const glm::vec3& currentVelocity = _velocityMatrix->get(x, y, z);
        _velocityMatrix->set(x, y, z, currentVelocity + dVelocity);
      }
    }
  }
}

void FluidObject::_blockVelocity()
{
  for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
      {
        glm::vec3& velocityRef = (*_velocityMatrix)(x, y, z);
        if (_blockMatrix->get(x, y, z) == 1)
        {
          velocityRef = glm::vec3(0.f);
        }

        if (_blockMatrix->get(x + 1, y, z) == 1 ||
            _blockMatrix->get(x - 1, y, z) == 1)
        {
          velocityRef.x = 0;
        }
        if (_blockMatrix->get(x, y - 1, z) == 1 ||
            _blockMatrix->get(x, y + 1, z) == 1)
        {
          velocityRef.y = 0;
        }
        if (_blockMatrix->get(x, y, z - 1) == 1 ||
            _blockMatrix->get(x, y, z + 1) == 1)
        {
          velocityRef.z = 0;
        }
      }
    }
  }
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
          float divirgence = 0;
          if (_blockMatrix->get(x + 1, y, z) == 0)
          {
            divirgence += _velocityMatrix->get(x + 1, y, z).x;
          }
          if (_blockMatrix->get(x - 1, y, z) == 0)
          {
            divirgence -= _velocityMatrix->get(x - 1, y, z).x;
          }
          if (_blockMatrix->get(x, y + 1, z) == 0)
          {
            divirgence += _velocityMatrix->get(x, y + 1, z).y;
          }
          if (_blockMatrix->get(x, y - 1, z) == 0)
          {
            divirgence -= _velocityMatrix->get(x, y - 1, z).y;
          }
          if (_blockMatrix->get(x, y, z + 1) == 0)
          {
            divirgence += _velocityMatrix->get(x, y, z + 1).z;
          }
          if (_blockMatrix->get(x, y, z - 1) == 0)
          {
            divirgence -= _velocityMatrix->get(x, y, z - 1).z;
          }
          target(x, y, z) = divirgence;
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
          float pressureAccum = 0;
          float divider = 0;

          if (_blockMatrix->get(x - 1, y, z) == 0)
          {
            pressureAccum += prew(x - 1, y, z);
            divider += 1.f;
          }
          if (_blockMatrix->get(x + 1, y, z) == 0)
          {
            pressureAccum += prew(x + 1, y, z);
            divider += 1.f;
          }
          if (_blockMatrix->get(x, y - 1, z) == 0)
          {
            pressureAccum += prew(x, y - 1, z);
            divider += 1.f;
          }
          if (_blockMatrix->get(x, y + 1, z) == 0)
          {
            pressureAccum += prew(x, y + 1, z);
            divider += 1.f;
          }
          if (_blockMatrix->get(x, y, z - 1) == 0)
          {
            pressureAccum += prew(x, y, z - 1);
            divider += 1.f;
          }
          if (_blockMatrix->get(x, y, z + 1) == 0)
          {
            pressureAccum += prew(x, y, z + 1);
            divider += 1.f;
          }
          if(divider == 0.f) next(x, y, z) = 0.f;
          else
          {
            next(x, y, z) = (pressureAccum - divirgence(x, y, z)) / divider;
          }
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
        if (_blockMatrix->get(x, y, z) != 0)
        {
          (*_velocityMatrix)(x, y, z) = glm::vec3(0.f);
          continue;
        }

        glm::vec3 deltaV(0.f);
        if (_blockMatrix->get(x - 1, y, z) == 0 &&
            _blockMatrix->get(x + 1, y, z) == 0)
        {
          deltaV.x = projPressure(x - 1, y, z) - projPressure(x + 1, y, z);
        }
        if (_blockMatrix->get(x, y - 1, z) == 0 &&
            _blockMatrix->get(x, y + 1, z) == 0)
        {
          deltaV.y = projPressure(x, y - 1, z) - projPressure(x, y + 1, z);
        }
        if (_blockMatrix->get(x, y, z - 1) == 0 &&
            _blockMatrix->get(x, y, z + 1) == 0)
        {
          deltaV.z = projPressure(x, y, z - 1) - projPressure(x, y, z + 1);
        }
        deltaV /= 2.f;
        (*_velocityMatrix)(x, y, z) += deltaV;
      }
    }
  }
}

void FluidObject::_moveMatrices(float dTime)
{
  FluidMatrix<glm::vec3> newVelocity( _velocityMatrix->xSize(),
                                      _velocityMatrix->ySize(),
                                      _velocityMatrix->zSize());
  newVelocity.clear(_wind);

  FluidMatrix<float> newTemperatureMatrix(_velocityMatrix->xSize(),
                                          _velocityMatrix->ySize(),
                                          _velocityMatrix->zSize());
  newTemperatureMatrix.clear(defaultTemperature);

  FluidMatrix<float> newPpressureMatrix(_velocityMatrix->xSize(),
                                        _velocityMatrix->ySize(),
                                        _velocityMatrix->zSize());
  newPpressureMatrix.clear(defaultPressure);

  for (size_t x = 1; x < newVelocity.xSize() - 1; x++)
  {
    for (size_t y = 1; y < newVelocity.ySize() - 1; y++)
    {
      for (size_t z = 1; z < newVelocity.zSize() - 1; z++)
      {
        glm::vec3 currentVelocity = _velocityMatrix->get(x, y, z);
        glm::vec3 shift = -currentVelocity / _cellSize * dTime;
        glm::vec3 shiftedCoord = glm::vec3(x, y, z) + shift + glm::vec3(.5f);
        newVelocity(x,y,z) = _velocityMatrix->interpolate(shiftedCoord);
        newTemperatureMatrix(x, y, z) =
                                  _temperatureMatrix->interpolate(shiftedCoord);
        newPpressureMatrix(x, y, z) =
                                    _pressureMatrix->interpolate(shiftedCoord);
      }
    }
  }

  _velocityMatrix->swap(newVelocity);
  _temperatureMatrix->swap(newTemperatureMatrix);
  _pressureMatrix->swap(newPpressureMatrix);
}

glm::vec3 FluidObject::_toMatrixCoord(
                                    const glm::vec3& fieldCoord) const noexcept
{
  glm::vec3 matrixCoord = fieldCoord + _parent.size() / 2.f;
  matrixCoord /= _cellSize;
  matrixCoord += 1.5f;
  return matrixCoord;
}

glm::vec3 FluidObject::_toFieldCoord(
                                    const glm::vec3& matrixCoord) const noexcept
{
  glm::vec3 fieldCoord = matrixCoord - 1.5f;
  fieldCoord *= _cellSize;
  fieldCoord -= _parent.size() / 2.f;
  return fieldCoord;
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
