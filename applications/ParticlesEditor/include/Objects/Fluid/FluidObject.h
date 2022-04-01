#pragma once

#include <vector>
#include <optional>

#include <glm/vec3.hpp>

#include <Objects/Fluid/FluidMatrix.h>
#include <Objects/ModificatorObject.h>

class BlockerObject;
class ParticleField;

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
              RESET resetTypeMask
              NOTIFY typeMaskChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float cellSize
              READ cellSize
              WRITE setCellSize
              RESET resetCellSize
              NOTIFY cellSizeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

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
  inline void resetTypeMask() noexcept;

  inline float cellSize() const noexcept;
  void setCellSize(float newValue) noexcept;
  inline void resetCellSize() noexcept;

  inline const glm::vec3& wind() const noexcept;
  void setWind(const glm::vec3& newValue) noexcept;
  inline void resetWind() noexcept;

  virtual void simulationStep(mtt::TimeT currentTime, mtt::TimeT delta);
  void clear() noexcept;

  void registerBlocker(BlockerObject& blocker);
  void unregisterBlocker(BlockerObject& blocker) noexcept;

  inline FluidMatrix<float>* temperatureMatrix() noexcept;
  inline const FluidMatrix<float>* temperatureMatrix() const noexcept;
  inline FluidMatrix<float>* pressureMatrix() noexcept;
  inline const FluidMatrix<float>* pressureMatrix() const noexcept;

  inline glm::vec3 toMatrixCoord(const glm::vec3& fieldCoord) const noexcept;
  inline glm::vec3 toFieldCoord(const glm::vec3& matrixCoord) const noexcept;

  inline float getCellMass(size_t x, size_t y, size_t z) const noexcept;

signals:
  void typeMaskChanged(uint32_t newValue);
  void cellSizeChanged(float newValue);
  void windChanged(glm::vec3 newValue);

private:
  void _resetMatrices() noexcept;
  void _resetBlockMatrix() noexcept;
  void _rebuildMatrices();
  void _applyBlocker(BlockerObject& blocker);
  void _rebuildBlockMatrix();
  void _calculateMassMatrix() noexcept;
  template<int direction>
  float _getVelocity(size_t x, size_t y, size_t z);
  template<int direction, int normal1, int normal2>
  glm::vec3 _getFrictionForce(size_t x, size_t y, size_t z);
  void _applyArchimedesForce(float dTime) noexcept;
  void _applyFriction(float dTime) noexcept;
  void _blockVelocity();
  void _projectVelocity();
  void _buildDivirgence(FluidMatrix<float>& target);
  void _buildProjPressure(FluidMatrix<float>& target,
                          const FluidMatrix<float>& divirgence);
  void _moveMatrices(float dTime);
  void _updateParticles(float dTime);

private:
  ParticleField& _parent;

  uint32_t _typeMask;
  float _cellSize;
  float _cellVolume;
  glm::vec3 _wind;

  std::optional<FluidMatrix<glm::vec3>> _velocityMatrix;
  std::optional<FluidMatrix<float>> _temperatureMatrix;
  std::optional<FluidMatrix<float>> _pressureMatrix;
  std::optional<FluidMatrix<float>> _massMatrix;
  std::optional<FluidMatrix<int>> _blockMatrix;

  using Blockers = std::vector<BlockerObject*>;
  Blockers _blockers;
};

inline uint32_t FluidObject::typeMask() const noexcept
{
  return _typeMask;
}

inline void FluidObject::resetTypeMask() noexcept
{
  setTypeMask(1);
}

inline float FluidObject::cellSize() const noexcept
{
  return _cellSize;
}

inline void FluidObject::resetCellSize() noexcept
{
  setCellSize(1.f);
}

inline const glm::vec3& FluidObject::wind() const noexcept
{
  return _wind;
}

inline void FluidObject::resetWind() noexcept
{
  setWind(glm::vec3(0.f));
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

inline float FluidObject::getCellMass(size_t x,
                                      size_t y,
                                      size_t z) const noexcept
{
  constexpr float gasConstant = 287.058f;
  float temperature = _temperatureMatrix->get(x, y, z);
  float pressure = _pressureMatrix->get(x, y, z);
  float density = pressure / (gasConstant * temperature);
  return _cellVolume * density;
}
