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

signals:
  void typeMaskChanged(uint32_t newValue);
  void cellSizeChanged(float newValue);
  void windChanged(glm::vec3 newValue);

private:
  glm::vec3 _toMatrixCoord(const glm::vec3& fieldCoord) const noexcept;
  glm::vec3 _toFieldCoord(const glm::vec3& matrixCoord) const noexcept;
  void _resetMatrices() noexcept;
  void _resetBlockMatrix() noexcept;
  void _rebuildMatrices();
  void _applyBlocker(BlockerObject& blocker);
  void _rebuildBlockMatrix();
  void _blockVelocity();
  void _projectVelocity();
  void _buildDivirgence(FluidMatrix<float>& target);
  void _buildProjPressure(FluidMatrix<float>& target,
                          const FluidMatrix<float>& divirgence);
  void _moveVelocity(float dTime);
  void _updateParticles(float dTime);

private:
  ParticleField& _parent;

  uint32_t _typeMask;
  float _cellSize;
  glm::vec3 _wind;

  std::optional<FluidMatrix<glm::vec3>> _velocityMatrix;
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
