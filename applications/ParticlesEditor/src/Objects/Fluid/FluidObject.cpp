#include <algorithm>

#include <mtt/utilities/Abort.h>
#include <mtt/utilities/Box.h>

#include <Objects/Fluid/BlockerObject.h>
#include <Objects/Fluid/FluidObject.h>
#include <Objects/ParticleField.h>

FluidObject::FluidObject( const QString& name,
                          bool canBeRenamed,
                          ParticleField& parent,
                          const mtt::UID& id) :
  mtt::Object(name, canBeRenamed, id),
  _parent(parent),
  _typeMask(1),
  _cellSize(1.f),
  _cellVolume(1.f),
  _wind(0.f),
  _solverIterations(30)
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

void FluidObject::setSolverIterations(size_t newValue) noexcept
{
  if(newValue < 1) newValue = 1;
  if(_solverIterations == newValue) return;
  _solverIterations = newValue;
  emit solverIterationsChanged(newValue);
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

  float dTime = mtt::toFloatTime(delta);

  if(dTime != 0.f)
  {
    _applyMixing(dTime);
    _calculateMassMatrix();
    _applyArchimedesForce(dTime);
    _applyFriction(dTime),
    _blockVelocity();
    _applyContinuityEquation(dTime);
    _moveMatrices(dTime);
    _applyContinuityEquation(dTime);

    _pressureMatrix->clear(defaultPressure);
  }

  _updateParticles(dTime);
}

void FluidObject::_applyNeighbourTemp(size_t x,
                                      size_t y,
                                      size_t z,
                                      const glm::vec3& flowSpeed,
                                      float dTime,
                                      float& temperatureAccumulator,
                                      float& weightAccumulator) noexcept
{
  if (_blockMatrix->get(x, y, z) != 0) return;

  glm::vec3 currentSpeed = _velocityMatrix->get(x,y,z);
  float dSpeed = glm::length(flowSpeed - currentSpeed);

  float weight = dTime * mixingFactor * dSpeed / _cellSize;
  weight = glm::clamp(0.f, 1.f, weight);

  temperatureAccumulator += _temperatureMatrix->get(x, y, z) * weight;
  weightAccumulator += weight;
}

void FluidObject::_applyMixing(float dTime)
{
  FluidMatrix<float> newTemperature(_velocityMatrix->xSize(),
                                    _velocityMatrix->ySize(),
                                    _velocityMatrix->zSize());
  newTemperature.clear(defaultTemperature);

  for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
      {
        if (_blockMatrix->get(x, y, z) != 0) continue;

        glm::vec3 flowSpeed = _velocityMatrix->get(x, y, z);

        float avgTemp = _temperatureMatrix->get(x,y ,z);
        float totalWeght = 1.f;

        _applyNeighbourTemp(x - 1, y, z, flowSpeed, dTime, avgTemp, totalWeght);
        _applyNeighbourTemp(x + 1, y, z, flowSpeed, dTime, avgTemp, totalWeght);
        _applyNeighbourTemp(x, y - 1, z, flowSpeed, dTime, avgTemp, totalWeght);
        _applyNeighbourTemp(x, y + 1, z, flowSpeed, dTime, avgTemp, totalWeght);
        _applyNeighbourTemp(x, y, z - 1, flowSpeed, dTime, avgTemp, totalWeght);
        _applyNeighbourTemp(x, y, z + 1, flowSpeed, dTime, avgTemp, totalWeght);

        newTemperature(x, y, z) = avgTemp / totalWeght;
      }
    }
  }

  _temperatureMatrix->swap(newTemperature);
}

void FluidObject::_calculateMassMatrix() noexcept
{
  for (size_t z = 0; z < _velocityMatrix->zSize(); z++)
  {
    for (size_t y = 0; y < _velocityMatrix->ySize(); y++)
    {
      for (size_t x = 0; x < _velocityMatrix->xSize(); x++)
      {
        float temperature = _temperatureMatrix->get(x, y, z);
        _massMatrix->set( x,
                          y,
                          z,
                          getGasMass( _cellVolume,
                                      defaultPressure,
                                      temperature));
      }
    }
  }
}

void FluidObject::_applyArchimedesForce(float dTime) noexcept
{
  for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
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

void FluidObject::_applyFriction(float dTime) noexcept
{
  FluidMatrix<glm::vec3> newVelocity( _velocityMatrix->xSize(),
                                      _velocityMatrix->ySize(),
                                      _velocityMatrix->zSize());
  newVelocity.clear(_wind);

  for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
      {
        if(_blockMatrix->get(x, y, z) != 0)
        {
          newVelocity(x, y, z) = glm::vec3(0.f);
          continue;
        }

        glm::vec3 envVelocity(0.f);
        if (_blockMatrix->get(x - 1, y, z) == 0)
        {
          envVelocity += _velocityMatrix->get(x - 1, y, z);
        }
        if (_blockMatrix->get(x + 1, y, z) == 0)
        {
          envVelocity += _velocityMatrix->get(x + 1, y, z);
        }
        if (_blockMatrix->get(x, y - 1, z) == 0)
        {
          envVelocity += _velocityMatrix->get(x, y - 1, z);
        }
        if (_blockMatrix->get(x, y + 1, z) == 0)
        {
          envVelocity += _velocityMatrix->get(x, y + 1, z);
        }
        if (_blockMatrix->get(x, y, z - 1) == 0)
        {
          envVelocity += _velocityMatrix->get(x, y, z - 1);
        }
        if (_blockMatrix->get(x, y, z + 1) == 0)
        {
          envVelocity += _velocityMatrix->get(x, y, z + 1);
        }
        envVelocity /= 6.f;
        envVelocity -= _velocityMatrix->get(x, y, z);

        glm::vec3 force = frictionFactor * _cellSize * envVelocity;
        float mass = _massMatrix->get(x, y, z);
        glm::vec3 dVelocity = force / mass * dTime;
        newVelocity(x, y, z) = _velocityMatrix->get(x, y, z) + dVelocity;
      }
    }
  }

  _velocityMatrix->swap(newVelocity);
}

void FluidObject::_blockVelocity()
{
  for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
      {
        glm::vec3& velocityRef = (*_velocityMatrix)(x, y, z);
        if (_blockMatrix->get(x, y, z) != 0)
        {
          velocityRef = glm::vec3(0.f);
        }

        if (_blockMatrix->get(x + 1, y, z) != 0 ||
            _blockMatrix->get(x - 1, y, z) != 0)
        {
          velocityRef.x = 0;
        }
        if (_blockMatrix->get(x, y - 1, z) != 0 ||
            _blockMatrix->get(x, y + 1, z) != 0)
        {
          velocityRef.y = 0;
        }
        if (_blockMatrix->get(x, y, z - 1) != 0 ||
            _blockMatrix->get(x, y, z + 1) != 0)
        {
          velocityRef.z = 0;
        }
      }
    }
  }
}

void FluidObject::_buildCorrectFieldDivirgence( FluidMatrix<float>& target,
                                                float dTime)
{
  for (size_t z = 0; z < _velocityMatrix->zSize(); z++)
  {
    for (size_t y = 0; y < _velocityMatrix->ySize(); y++)
    {
      for (size_t x = 0; x < _velocityMatrix->xSize(); x++)
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
            divirgence += _velocityMatrix->get(x + 1, y, z).x *
                                              _pressureMatrix->get(x + 1, y, z);
          }
          if (_blockMatrix->get(x - 1, y, z) == 0)
          {
            divirgence -= _velocityMatrix->get(x - 1, y, z).x *
                                              _pressureMatrix->get(x - 1, y, z);
          }
          if (_blockMatrix->get(x, y + 1, z) == 0)
          {
            divirgence += _velocityMatrix->get(x, y + 1, z).y *
                                              _pressureMatrix->get(x, y + 1, z);
          }
          if (_blockMatrix->get(x, y - 1, z) == 0)
          {
            divirgence -= _velocityMatrix->get(x, y - 1, z).y *
                                              _pressureMatrix->get(x, y - 1, z);
          }
          if (_blockMatrix->get(x, y, z + 1) == 0)
          {
            divirgence += _velocityMatrix->get(x, y, z + 1).z *
                                              _pressureMatrix->get(x, y, z + 1);
          }
          if (_blockMatrix->get(x, y, z - 1) == 0)
          {
            divirgence -= _velocityMatrix->get(x, y, z - 1).z *
                                              _pressureMatrix->get(x, y, z - 1);
          }
          divirgence /= _cellSize;

          float dPressure = defaultPressure - _pressureMatrix->get(x, y, z);
          dPressure /= dTime;

          divirgence += dPressure;

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
      iteration < _solverIterations;
      iteration++)
  {
    for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
    {
      for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
      {
        for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
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
            next(x, y, z) = (pressureAccum + divirgence(x, y, z)) / divider;
          }
        }
      }
    }
    prew.swap(next);
  }

  target.swap(prew);
}

void FluidObject::_applyContinuityEquation(float dTime)
{
  FluidMatrix<float> divirgence(_velocityMatrix->xSize(),
                                _velocityMatrix->ySize(),
                                _velocityMatrix->zSize());
  _buildCorrectFieldDivirgence(divirgence, dTime);
  FluidMatrix<float> projPressure(_velocityMatrix->xSize(),
                                  _velocityMatrix->ySize(),
                                  _velocityMatrix->zSize());
  _buildProjPressure(projPressure, divirgence);

  for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
      {
        if (_blockMatrix->get(x, y, z) != 0)
        {
          (*_velocityMatrix)(x, y, z) = glm::vec3(0.f);
          continue;
        }

        glm::vec3 pVCorrection(0.f);
        if (_blockMatrix->get(x - 1, y, z) == 0 &&
            _blockMatrix->get(x + 1, y, z) == 0)
        {
          pVCorrection.x =
                          projPressure(x - 1, y, z) - projPressure(x + 1, y, z);
        }
        if (_blockMatrix->get(x, y - 1, z) == 0 &&
            _blockMatrix->get(x, y + 1, z) == 0)
        {
          pVCorrection.y =
                          projPressure(x, y - 1, z) - projPressure(x, y + 1, z);
        }
        if (_blockMatrix->get(x, y, z - 1) == 0 &&
            _blockMatrix->get(x, y, z + 1) == 0)
        {
          pVCorrection.z =
                          projPressure(x, y, z - 1) - projPressure(x, y, z + 1);
        }
        pVCorrection *= _cellSize;
        pVCorrection /= 2.f;
        glm::vec3 vCorrection = pVCorrection / _pressureMatrix->get(x, y, z);
        (*_velocityMatrix)(x, y, z) -= vCorrection;
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

  for (size_t z = 1; z < newVelocity.zSize() - 1; z++)
  {
    for (size_t y = 1; y < newVelocity.ySize() - 1; y++)
    {
      for (size_t x = 1; x < newVelocity.xSize() - 1; x++)
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
