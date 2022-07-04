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

  void emitEnergy(float energy);

  virtual void simulationStep(mtt::TimeRange time) override;

signals:
  void powerChanged(float newValue);

private:
  float _power;
};

inline float HeaterObject::power() const noexcept
{
  return _power;
}
