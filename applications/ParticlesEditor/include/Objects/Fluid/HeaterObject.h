#pragma once

#include <Objects/Fluid/FluidModificator.h>

class HeaterObject : public FluidModificator
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitHeaterObject,
                          visitConstHeaterObject,
                          FluidModificator)

  Q_PROPERTY( float power
              READ power
              WRITE setPower
              RESET resetPower
              NOTIFY powerChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  HeaterObject( const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  HeaterObject(const HeaterObject&) = delete;
  HeaterObject& operator = (const HeaterObject&) = delete;
  virtual ~HeaterObject() noexcept = default;

  inline float power() const noexcept;
  void setPower(float newValue) noexcept;
  inline void resetPower() noexcept;

  virtual void simulationStep(mtt::TimeT currentTime,
                              mtt::TimeT deltaT) override;

signals:
  void powerChanged(float newValue);

private:
  float _power;
};

inline float HeaterObject::power() const noexcept
{
  return _power;
}

inline void HeaterObject::resetPower() noexcept
{
  setPower(1000.f);
}
