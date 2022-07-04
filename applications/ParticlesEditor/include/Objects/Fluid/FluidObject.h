#pragma once

#include <vector>
#include <optional>

#include <glm/vec3.hpp>

#include <mtt/application/Scene/Object.h>
#include <mtt/application/TimeT.h>
#include <mtt/utilities/Box.h>

#include <Objects/ParticleField.h>
#include <Objects/Fluid/FluidMatrix.h>
#include <Objects/PEVisitorExtension.h>

class BlockerObject;

class FluidObject : public mtt::Object
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitFluidObject,
                          visitConstFluidObject,
                          mtt::Object)

  Q_PROPERTY( uint32_t typeMask
              READ typeMask
              WRITE setTypeMask
              NOTIFY typeMaskChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float cellSize
              READ cellSize
              WRITE setCellSize
              NOTIFY cellSizeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( glm::vec3 wind
              READ wind
              WRITE setWind
              NOTIFY windChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( size_t solverIterations
              READ solverIterations
              WRITE setSolverIterations
              NOTIFY solverIterationsChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  static constexpr float defaultTemperature = 273.15f;
  static constexpr float defaultPressure = 100000.f;
  static constexpr float frictionFactor = .02f;
  static constexpr float mixingFactor = 1.f;
  static constexpr float heatCapacity = 1000.f;

public:
  FluidObject(const QString& name,
              bool canBeRenamed,
              ParticleField& parent,
              const mtt::UID& id = mtt::UID());
  FluidObject(const FluidObject&) = delete;
  FluidObject& operator = (const FluidObject&) = delete;
  virtual ~FluidObject() noexcept = default;

  inline uint32_t typeMask() const noexcept;
  void setTypeMask(uint32_t newValue) noexcept;

  inline float cellSize() const noexcept;
  void setCellSize(float newValue) noexcept;

  inline float cellVolume() noexcept;

  inline const glm::vec3& wind() const noexcept;
  void setWind(const glm::vec3& newValue) noexcept;

  inline size_t solverIterations() const noexcept;
  void setSolverIterations(size_t newValue) noexcept;

  virtual void simulationStep(mtt::TimeRange time);
  void clear() noexcept;

  void registerBlocker(BlockerObject& blocker);
  void unregisterBlocker(BlockerObject& blocker) noexcept;

  inline FluidMatrix<float>* temperatureMatrix() noexcept;
  inline const FluidMatrix<float>* temperatureMatrix() const noexcept;
  inline FluidMatrix<float>* pressureMatrix() noexcept;
  inline const FluidMatrix<float>* pressureMatrix() const noexcept;

  inline glm::vec3 toMatrixCoord(const glm::vec3& fieldCoord) const noexcept;
  inline glm::vec3 toFieldCoord(const glm::vec3& matrixCoord) const noexcept;

  inline float getGasMass(float volume,
                          float pressure,
                          float temperature) const noexcept;
  inline float getCellMass(size_t x, size_t y, size_t z) const noexcept;

  /// area in field coords
  /// theDelegate should have signature void (size_t xCellIndex,
  ///                                         size_t yCellIndex,
  ///                                         size_t zCellIndex,
  ///                                         const glm::vec3& cellPosition)
  /// cellPosition in field coords
  template <typename Delegate>
  inline void passCells(const mtt::Box& area, Delegate theDelegate);

signals:
  void typeMaskChanged(uint32_t newValue);
  void cellSizeChanged(float newValue);
  void windChanged(glm::vec3 newValue);
  void solverIterationsChanged(size_t newValue);

private:
  void _resetMatrices() noexcept;
  void _resetBlockMatrix() noexcept;
  void _rebuildMatrices();
  void _applyBlocker(BlockerObject& blocker);
  void _rebuildBlockMatrix();
  void _applyNeighbourTemp( size_t x,
                            size_t y,
                            size_t z,
                            const glm::vec3& flowSpeed,
                            float dTime,
                            float& temperatureAccumulator,
                            float& weightAccumulator) noexcept;
  void _applyForcesStep(FluidMatrix<float>& newTemperature,
                        FluidMatrix<glm::vec3>& newVelocity,
                        size_t startZ,
                        size_t finishZ,
                        float dTime);
  void _applyForces(float dTime);
  template <typename Func>
  void _makeAsync(Func func);

  void _applyProjPressureStep(size_t startZ,
                              size_t finishZ,
                              float dTime);
  void _applyContinuityEquation(float dTime);

  void _buildDivirgenceStep(FluidMatrix<float>&target,
                            size_t startZ,
                            size_t finishZ,
                            float dTime);
  void _buildCorrectFieldDivirgence(FluidMatrix<float>& target, float dTime);
  void _resolvePressureStep(const FluidMatrix<float>& prew,
                            FluidMatrix<float>& next,
                            const FluidMatrix<float>& divirgence,
                            size_t startZ,
                            size_t finishZ) const;
  void _buildProjPressure(FluidMatrix<float>& target,
                          const FluidMatrix<float>& divirgence);
  void _moveMatricesStep(size_t startZ, size_t finishZ, float dTime);
  void _moveMatrices(float dTime);
  void _updateParticles(float dTime);

private:
  ParticleField& _parent;

  uint32_t _typeMask;
  float _cellSize;
  float _cellVolume;
  glm::vec3 _wind;

  size_t _solverIterations;

  std::optional<FluidMatrix<glm::vec3>> _velocityMatrix;
  std::optional<FluidMatrix<glm::vec3>> _nextVelocityMatrix;
  std::optional<FluidMatrix<float>> _temperatureMatrix;
  std::optional<FluidMatrix<float>> _nextTemperatureMatrix;
  std::optional<FluidMatrix<float>> _pressureMatrix;
  std::optional<FluidMatrix<int>> _blockMatrix;

  std::optional<FluidMatrix<float>> _divirgenceMatrix;
  std::optional<FluidMatrix<float>> _projPressureMatrix;
  std::optional<FluidMatrix<float>> _resolveMatrix1;
  std::optional<FluidMatrix<float>> _resolveMatrix2;

  std::optional<FluidMatrix<glm::vec3>> _movedVelocityMatrix;
  std::optional<FluidMatrix<float>> _movedTemperatureMatrix;

  using Blockers = std::vector<BlockerObject*>;
  Blockers _blockers;
};

inline uint32_t FluidObject::typeMask() const noexcept
{
  return _typeMask;
}

inline float FluidObject::cellSize() const noexcept
{
  return _cellSize;
}

inline float FluidObject::cellVolume() noexcept
{
  return _cellVolume;
}

inline const glm::vec3& FluidObject::wind() const noexcept
{
  return _wind;
}

inline size_t FluidObject::solverIterations() const noexcept
{
  return _solverIterations;
}

inline FluidMatrix<float>* FluidObject::temperatureMatrix() noexcept
{
  if(_temperatureMatrix.has_value()) return &_temperatureMatrix.value();
  return nullptr;
}

inline const FluidMatrix<float>* FluidObject::temperatureMatrix() const noexcept
{
  if (_temperatureMatrix.has_value()) return &_temperatureMatrix.value();
  return nullptr;
}

inline FluidMatrix<float>* FluidObject::pressureMatrix() noexcept
{
  if (_pressureMatrix.has_value()) return &_pressureMatrix.value();
  return nullptr;
}

inline const FluidMatrix<float>* FluidObject::pressureMatrix() const noexcept
{
  if (_pressureMatrix.has_value()) return &_pressureMatrix.value();
  return nullptr;
}

inline glm::vec3 FluidObject::toMatrixCoord(
                                    const glm::vec3& fieldCoord) const noexcept
{
  glm::vec3 matrixCoord = fieldCoord + _parent.size() / 2.f;
  matrixCoord /= _cellSize;
  matrixCoord += 1.5f;
  return matrixCoord;
}

inline glm::vec3 FluidObject::toFieldCoord(
                                    const glm::vec3& matrixCoord) const noexcept
{
  glm::vec3 fieldCoord = matrixCoord - 1.5f;
  fieldCoord *= _cellSize;
  fieldCoord -= _parent.size() / 2.f;
  return fieldCoord;
}

inline float FluidObject::getGasMass( float volume,
                                      float pressure,
                                      float temperature) const noexcept
{
  constexpr float gasConstant = 287.058f;
  float density = pressure / (gasConstant * temperature);
  return density * volume;
}

inline float FluidObject::getCellMass(size_t x,
                                      size_t y,
                                      size_t z) const noexcept
{
  float temperature = _temperatureMatrix->get(x, y, z);
  float pressure = _pressureMatrix->get(x, y, z);
  return getGasMass(_cellVolume, pressure, temperature);
}

template <typename Delegate>
inline void FluidObject::passCells(const mtt::Box& area, Delegate theDelegate)
{
  if (!area.valid()) return;
  if (!_velocityMatrix.has_value()) _rebuildMatrices();
  if (!_velocityMatrix.has_value()) return;

  glm::vec3 startCorner = glm::floor(toMatrixCoord(area.minCorner));
  startCorner = glm::max(startCorner, glm::vec3(1.f));
  size_t startX = glm::min(size_t(startCorner.x), _blockMatrix->xSize() - 2);
  size_t startY = glm::min(size_t(startCorner.y), _blockMatrix->ySize() - 2);
  size_t startZ = glm::min(size_t(startCorner.z), _blockMatrix->zSize() - 2);

  glm::vec3 endCorner = glm::floor(toMatrixCoord(area.maxCorner));
  endCorner += glm::vec3(1.f);
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
        theDelegate(x, y, z, toFieldCoord(cellPosition));
        cellPosition.z += 1.f;
      }
      cellPosition.y += 1.f;
    }
    cellPosition.x += 1.f;
  }
}
