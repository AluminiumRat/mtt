#include <algorithm>

#include <mtt/utilities/Abort.h>
#include <mtt/utilities/Box.h>

#include <Objects/Fluid/BlockerObject.h>
#include <Objects/Fluid/FluidObject.h>
#include <Objects/ParticleField.h>

#define PROJECT_ITERATIONS 30

FluidObject::FluidObject( const QString& name,
                          bool canBeRenamed,
                          ParticleField& parent,
                          const mtt::UID& id) :
  mtt::Object(name, canBeRenamed, id),
  _parent(parent),
  _typeMask(1),
  _cellSize(1.f),
  _cellVolume(1.f),
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
  _cellVolume = _cellSize * _cellSize * _cellSize;
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
  mtt::Box blockerBound(glm::vec3(-halfSize), glm::vec3(halfSize));
  mtt::Box localBlockerBox = blockerBound.translated(fromBlocker);

  auto blockerDelegate =
    [&](size_t x, size_t y, size_t z, const glm::vec3& fieldCoord)
    {
      glm::vec3 blockerCoord = toBlocker * glm::vec4(fieldCoord, 1.f);
      if (blocker.isPointInside(blockerCoord)) _blockMatrix->set(x, y, z, 1);
    };

  passCells(localBlockerBox, blockerDelegate);
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
  for (size_t x = 0; x < _velocityMatrix->xSize(); x++)
  {
    for (size_t y = 0; y < _velocityMatrix->ySize(); y++)
    {
      for (size_t z = 0; z < _velocityMatrix->zSize(); z++)
      {
        _massMatrix->set(x, y, z, getCellMass(x, y, z));
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

  glm::vec3 force(0.f);
  force[direction] = velocity * frictionFactor * _cellSize;

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

void FluidObject::_updateParticles(float dTime)
{
  if(dTime <= 0.f) return;

  auto theDelegate =
    [&](ParticleField::ParticleData& particle)
    {
      if(particle.frictionFactor <= 0.f) return;

      glm::vec3 matrixCoord = toMatrixCoord(particle.position);
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
