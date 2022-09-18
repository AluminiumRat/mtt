#include <algorithm>
#include <future>
#include <thread>

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

  connect(&parent,
          &ParticleField::transformChanged,
          this,
          &FluidObject::_resetBlockMatrix,
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
  _nextVelocityMatrix.reset();
  _temperatureMatrix.reset();
  _nextTemperatureMatrix.reset();
  _pressureMatrix.reset();
  _divirgenceMatrix.reset();
  _projPressureMatrix.reset();
  _resolveMatrix1.reset();
  _resolveMatrix2.reset();
  _weightMatrix.reset();
  _movedVelocityMatrix.reset();
  _movedTemperatureMatrix.reset();
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

    _nextVelocityMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);

    _temperatureMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _temperatureMatrix->clear(defaultTemperature);

    _nextTemperatureMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);

    _pressureMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _pressureMatrix->clear(defaultPressure);

    _divirgenceMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _projPressureMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _resolveMatrix1.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _resolveMatrix2.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _weightMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _movedVelocityMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
    _movedTemperatureMatrix.emplace(fieldExtentX, fieldExtentY, fieldExtentZ);
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

void FluidObject::simulationStep(mtt::TimeRange time)
{
  if(!_velocityMatrix.has_value()) _rebuildMatrices();
  if(!_velocityMatrix.has_value()) return;

  if(!_blockMatrix.has_value()) _rebuildBlockMatrix();

  float dTime = mtt::toFloatTime(time.length());

  if(dTime != 0.f)
  {
    _applyForces(dTime);
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
  glm::vec3 currentSpeed = _velocityMatrix->get(x,y,z);
  float dSpeed = glm::length(flowSpeed - currentSpeed);

  float weight = dTime * mixingFactor * dSpeed / _cellSize;
  weight = glm::clamp(0.f, 1.f, weight);

  temperatureAccumulator += _temperatureMatrix->get(x, y, z) * weight;
  weightAccumulator += weight;
}

void FluidObject::_applyForcesStep( FluidMatrix<float>& newTemperature,
                                    FluidMatrix<glm::vec3>& newVelocity,
                                    size_t startZ,
                                    size_t finishZ,
                                    float dTime)
{
  for (size_t z = startZ; z < finishZ; z++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
      {
        if (_blockMatrix->get(x, y, z) != 0)
        {
          newVelocity(x, y, z) = glm::vec3(0.f);
          continue;
        }

        glm::vec3 flowSpeed = _velocityMatrix->get(x, y, z);

        float avgTemp = _temperatureMatrix->get(x, y, z);
        float totalTempWeght = 1.f;

        float neighbourMass = 0.f;
        float arhimedesNeighbours = 0.f;

        glm::vec3 envVelocity(0.f);

        if (_blockMatrix->get(x - 1, y, z) == 0)
        {
          _applyNeighbourTemp(x - 1,
                              y,
                              z,
                              flowSpeed,
                              dTime,
                              avgTemp,
                              totalTempWeght);
          neighbourMass += getGasMass(_cellVolume,
                                      defaultPressure,
                                      _temperatureMatrix->get(x - 1, y, z));
          arhimedesNeighbours += 1;
          envVelocity += _velocityMatrix->get(x - 1, y, z);
        }
        else flowSpeed.x = 0;

        if (_blockMatrix->get(x + 1, y, z) == 0)
        {
          _applyNeighbourTemp(x + 1,
                              y,
                              z,
                              flowSpeed,
                              dTime,
                              avgTemp,
                              totalTempWeght);
          neighbourMass += getGasMass(_cellVolume,
                                      defaultPressure,
                                      _temperatureMatrix->get(x + 1, y, z));
          arhimedesNeighbours += 1;
          envVelocity += _velocityMatrix->get(x + 1, y, z);
        }
        else flowSpeed.x = 0;

        if (_blockMatrix->get(x, y - 1, z) == 0)
        {
          _applyNeighbourTemp(x,
                              y - 1,
                              z,
                              flowSpeed,
                              dTime,
                              avgTemp,
                              totalTempWeght);
          neighbourMass += getGasMass(_cellVolume,
                                      defaultPressure,
                                      _temperatureMatrix->get(x, y - 1, z));
          arhimedesNeighbours += 1;
          envVelocity += _velocityMatrix->get(x, y - 1, z);
        }
        else flowSpeed.y = 0;

        if (_blockMatrix->get(x, y + 1, z) == 0)
        {
          _applyNeighbourTemp(x,
                              y + 1,
                              z,
                              flowSpeed,
                              dTime,
                              avgTemp,
                              totalTempWeght);
          neighbourMass += getGasMass(_cellVolume,
                                      defaultPressure,
                                      _temperatureMatrix->get(x, y + 1, z));
          arhimedesNeighbours += 1;
          envVelocity += _velocityMatrix->get(x, y + 1, z);
        }
        else flowSpeed.y = 0;

        if (_blockMatrix->get(x, y, z - 1) == 0)
        {
          _applyNeighbourTemp(x,
                              y,
                              z - 1,
                              flowSpeed,
                              dTime,
                              avgTemp,
                              totalTempWeght);
          envVelocity += _velocityMatrix->get(x, y, z - 1);
        }
        else flowSpeed.z = 0;

        if (_blockMatrix->get(x, y, z + 1) == 0)
        {
          _applyNeighbourTemp(x,
                              y,
                              z + 1,
                              flowSpeed,
                              dTime,
                              avgTemp,
                              totalTempWeght);
          envVelocity += _velocityMatrix->get(x, y, z + 1);
        }
        else flowSpeed.z = 0;

        newTemperature(x, y, z) = avgTemp / totalTempWeght;

        glm::vec3 dVelocity(0.f);

        float currentMass = getGasMass( _cellVolume,
                                        defaultPressure,
                                        _temperatureMatrix->get(x, y, z));

        if(arhimedesNeighbours != 0.f)
        {
          float force = neighbourMass / arhimedesNeighbours - currentMass;
          force *= 9.8f;
          float acceleration = force / currentMass;
          dVelocity += glm::vec3(0.f, 0.f, 1.f) * acceleration * dTime;
        }

        envVelocity /= 6.f;
        envVelocity -= flowSpeed;

        glm::vec3 frictionForce = frictionFactor * _cellSize * envVelocity;
        dVelocity += frictionForce / currentMass * dTime;

        newVelocity.set(x, y, z, flowSpeed + dVelocity);
      }
    }
  }
}

void FluidObject::_applyForces(float dTime)
{
  _nextTemperatureMatrix->clear(defaultTemperature);
  _nextVelocityMatrix->clear(_wind);

  auto stepFunc =
    [&](size_t startZ, size_t finishZ)
    {
      _applyForcesStep( *_nextTemperatureMatrix,
                        *_nextVelocityMatrix,
                        startZ,
                        finishZ,
                        dTime);
    };
  _makeAsync(stepFunc);

  _temperatureMatrix->swap(*_nextTemperatureMatrix);
  _velocityMatrix->swap(*_nextVelocityMatrix);
}

template <typename Func>
void FluidObject::_makeAsync(Func func)
{
  unsigned int threadsNumber = std::thread::hardware_concurrency();
  threadsNumber = std::max(threadsNumber, 8u);

  std::vector<std::future<void>> futures;

  size_t zLayersNumber = _velocityMatrix->zSize() - 2;
  size_t layerPerThread = zLayersNumber / threadsNumber;
  size_t remainder = zLayersNumber % threadsNumber;

  size_t zIndex = 1;

  for (unsigned int threadIndex = 0; threadIndex < threadsNumber; threadIndex++)
  {
    size_t layersNumber = layerPerThread;
    if(remainder != 0)
    {
      layersNumber++;
      remainder--;
    }

    size_t nextZIndex = zIndex + layersNumber;

    auto stepFunc =
      [&, zIndex, nextZIndex]()
      {
        func(zIndex, nextZIndex);
      };
    futures.push_back(std::async(stepFunc));

    zIndex = nextZIndex;
  }

  for(std::future<void>& future : futures) future.get();
}

void FluidObject::_buildDivirgenceStep( FluidMatrix<float>& target,
                                        size_t startZ,
                                        size_t finishZ,
                                        float dTime)
{
  for (size_t z = startZ; z < finishZ; z++)
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

void FluidObject::_buildCorrectFieldDivirgence( FluidMatrix<float>& target,
                                                float dTime)
{
  auto stepFunc =
    [&](size_t startZ, size_t finishZ)
    {
      _buildDivirgenceStep( target,
                            startZ,
                            finishZ,
                            dTime);
    };
  _makeAsync(stepFunc);
}

void FluidObject::_resolvePressureStep( const FluidMatrix<float>& prew,
                                        FluidMatrix<float>& next,
                                        const FluidMatrix<float>& divirgence,
                                        size_t startZ,
                                        size_t finishZ) const
{
  for (size_t z = startZ; z < finishZ - 1; z++)
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
}

void FluidObject::_buildProjPressure( FluidMatrix<float>& target,
                                      const FluidMatrix<float>& divirgence)
{
  _resolveMatrix1->clear(0.f);

  for(int iteration = 0;
      iteration < _solverIterations;
      iteration++)
  {
    auto stepFunc =
      [&](size_t startZ, size_t finishZ)
      {
        _resolvePressureStep( *_resolveMatrix1,
                              *_resolveMatrix2,
                              divirgence,
                              startZ,
                              finishZ);
      };
    _makeAsync(stepFunc);

    _resolveMatrix1->swap(*_resolveMatrix2);
  }

  target.swap(*_resolveMatrix1);
}

void FluidObject::_applyProjPressureStep( size_t startZ,
                                          size_t finishZ,
                                          float dTime)
{
  for (size_t z = startZ; z < finishZ; z++)
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
          pVCorrection.x = (* _projPressureMatrix)(x - 1, y, z) -
                                            (*_projPressureMatrix)(x + 1, y, z);
        }
        if (_blockMatrix->get(x, y - 1, z) == 0 &&
            _blockMatrix->get(x, y + 1, z) == 0)
        {
          pVCorrection.y = (*_projPressureMatrix)(x, y - 1, z) -
                                            (*_projPressureMatrix)(x, y + 1, z);
        }
        if (_blockMatrix->get(x, y, z - 1) == 0 &&
            _blockMatrix->get(x, y, z + 1) == 0)
        {
          pVCorrection.z = (*_projPressureMatrix)(x, y, z - 1) -
                                            (*_projPressureMatrix)(x, y, z + 1);
        }
        pVCorrection *= _cellSize;
        pVCorrection /= 2.f;
        glm::vec3 vCorrection = pVCorrection / _pressureMatrix->get(x, y, z);
        (*_velocityMatrix)(x, y, z) -= vCorrection;
      }
    }
  }
}

void FluidObject::_applyContinuityEquation(float dTime)
{
  _buildCorrectFieldDivirgence(*_divirgenceMatrix, dTime);
  _buildProjPressure(*_projPressureMatrix, *_divirgenceMatrix);

  auto stepFunc =
    [&](size_t startZ, size_t finishZ)
    {
      _applyProjPressureStep(startZ, finishZ, dTime);
    };
  _makeAsync(stepFunc);
}

void FluidObject::_clearMovedMatrices()
{
  _movedVelocityMatrix->clear(_wind);
  _movedTemperatureMatrix->clear(defaultTemperature);
  for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
      {
        _movedVelocityMatrix->set(x, y, z, glm::vec3(0.f));
        _movedTemperatureMatrix->set(x, y, z, 0.f);
      }
    }
  }
  _weightMatrix->clear(0.f);
}

inline void FluidObject::_moveToCell( int x,
                                      int y,
                                      int z,
                                      glm::vec3 velocity,
                                      float temperature,
                                      float weight)
{
  if( x > 0 &&
      x < _velocityMatrix->xSize() - 1 &&
      y > 0 &&
      y < _velocityMatrix->ySize() - 1 &&
      z > 0 &&
      z < _velocityMatrix->zSize() - 1)
  {
    (*_movedVelocityMatrix)(x, y, z) += velocity * weight;
    (*_movedTemperatureMatrix)(x, y, z) += temperature * weight;
    (*_weightMatrix)(x, y, z) += weight;
  }
}

void FluidObject::_moveMatrices(float dTime)
{
  _clearMovedMatrices();

  size_t xSize = _velocityMatrix->xSize();
  size_t ySize = _velocityMatrix->ySize();
  size_t zSize = _velocityMatrix->zSize();

  for (size_t z = 0; z < zSize; z++)
  {
    for (size_t y = 0; y < ySize; y++)
    {
      for (size_t x = 0; x < xSize; x++)
      {
        float temperature = _temperatureMatrix->get(x, y, z);
        glm::vec3 velocity = _velocityMatrix->get(x, y, z);
        glm::vec3 shift = velocity / _cellSize * dTime;
        glm::vec3 shiftedCoord = glm::vec3(x, y, z) + shift;

        size_t intX = size_t(shiftedCoord.x);
        float fractX = glm::fract(shiftedCoord.x);

        size_t intY = size_t(shiftedCoord.y);
        float fractY = glm::fract(shiftedCoord.y);

        size_t intZ = size_t(shiftedCoord.z);
        float fractZ = glm::fract(shiftedCoord.z);

        float weight = (1.f - fractX) * (1.f - fractY) * (1.f - fractZ);
        _moveToCell(intX, intY, intZ, velocity, temperature, weight);

        weight = fractX * (1.f - fractY) * (1.f - fractZ);
        _moveToCell(intX + 1, intY, intZ, velocity, temperature, weight);

        weight = (1.f - fractX) * fractY * (1.f - fractZ);
        _moveToCell(intX, intY + 1, intZ, velocity, temperature, weight);

        weight = fractX * fractY * (1.f - fractZ);
        _moveToCell(intX + 1, intY + 1, intZ, velocity, temperature, weight);

        weight = (1.f - fractX) * (1.f - fractY) * fractZ;
        _moveToCell(intX, intY, intZ + 1, velocity, temperature, weight);

        weight = fractX * (1.f - fractY) * fractZ;
        _moveToCell(intX + 1, intY, intZ + 1, velocity, temperature, weight);

        weight = (1.f - fractX) * fractY * fractZ;
        _moveToCell(intX, intY + 1, intZ + 1, velocity, temperature, weight);

        weight = fractX * fractY * fractZ;
        _moveToCell(intX + 1,
                    intY + 1,
                    intZ + 1,
                    velocity,
                    temperature,
                    weight);
      }
    }
  }

  _normalizeMovedMatrices();

  _velocityMatrix->swap(*_movedVelocityMatrix);
  _temperatureMatrix->swap(*_movedTemperatureMatrix);
}

void FluidObject::_normalizeMovedMatrices()
{
  for (size_t z = 1; z < _velocityMatrix->zSize() - 1; z++)
  {
    for (size_t y = 1; y < _velocityMatrix->ySize() - 1; y++)
    {
      for (size_t x = 1; x < _velocityMatrix->xSize() - 1; x++)
      {
        float weight = _weightMatrix->get(x, y, z);
        if(weight != 0.f)
        {
          (*_movedVelocityMatrix)(x, y, z) /= weight;
          (*_movedTemperatureMatrix)(x, y, z) /= weight;
        }
        else
        {
          (*_movedVelocityMatrix)(x, y, z) = _velocityMatrix->get(x, y, z);
          (*_movedTemperatureMatrix)(x, y, z) = _temperatureMatrix->get(x, y, z);
        }
      }
    }
  }
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
