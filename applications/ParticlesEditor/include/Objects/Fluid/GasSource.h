#pragma once

#include <Objects/Fluid/FluidModificator.h>
#include <Objects/Fluid/FluidObject.h>

class GasSource : public FluidModificator
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitGasSource,
                          visitConstGasSource,
                          FluidModificator)

  Q_PROPERTY( float flowRate
              READ flowRate
              WRITE setFlowRate
              RESET resetFlowRate
              NOTIFY flowRateChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float temperature
              READ temperature
              WRITE setTemperature
              RESET resetTemperature
              NOTIFY temperatureChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  GasSource(const QString& name,
            bool canBeRenamed,
            const mtt::UID& id = mtt::UID());
  GasSource(const GasSource&) = delete;
  GasSource& operator = (const GasSource&) = delete;
  virtual ~GasSource() noexcept = default;

  inline float flowRate() const noexcept;
  void setFlowRate(float newValue) noexcept;
  inline void resetFlowRate() noexcept;

  inline float temperature() const noexcept;
  void setTemperature(float newValue) noexcept;
  inline void resetTemperature() noexcept;

  void emitGas(float volume);

  virtual void simulationStep(mtt::TimeRange time) override;

signals:
  void flowRateChanged(float newValue);
  void temperatureChanged(float newValue);

private:
  float _flowRate;
  float _temperature;
};

inline float GasSource::flowRate() const noexcept
{
  return _flowRate;
}

inline void GasSource::resetFlowRate() noexcept
{
  setFlowRate(0.f);
}

inline float GasSource::temperature() const noexcept
{
  return _temperature;
}

inline void GasSource::resetTemperature() noexcept
{
  setTemperature(FluidObject::defaultTemperature);
}
