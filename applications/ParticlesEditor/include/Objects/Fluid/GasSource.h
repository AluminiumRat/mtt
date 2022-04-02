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

  virtual void simulationStep(mtt::TimeT currentTime,
                              mtt::TimeT deltaT) override;

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
