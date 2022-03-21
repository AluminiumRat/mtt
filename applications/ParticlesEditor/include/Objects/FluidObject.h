#pragma once

#include <Objects/ModificatorObject.h>

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

  virtual void simulationStep(mtt::TimeT currentTime, mtt::TimeT delta);

signals:
  void typeMaskChanged(uint32_t newValue);
  void cellSizeChanged(float newValue);

private:
  ParticleField& _parent;

  uint32_t _typeMask;
  float _cellSize;
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
