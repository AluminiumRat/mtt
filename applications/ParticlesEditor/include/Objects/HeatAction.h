#pragma once

#include <Objects/Fluid/HeaterObject.h>
#include <Objects/ActionAnimationTrack.h>

class HeatAction : public ActionAnimationTrack
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitHeatAction,
                          visitConstHeatAction,
                          ActionAnimationTrack)

  Q_PROPERTY( uint emittedEnergy
              READ emittedEnergy
              WRITE setEmittedEnergy
              RESET resetEmittedEnergy
              NOTIFY emittedEnergyChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  HeatAction( const QString& name,
              bool canBeRenamed,
              const mtt::UID& id = mtt::UID());
  HeatAction(const HeatAction&) = delete;
  HeatAction& operator = (const HeatAction&) = delete;
  virtual ~HeatAction() noexcept = default;

  inline float emittedEnergy() const noexcept;
  void setEmittedEnergy(float newValue) noexcept;
  inline void resetEmittedEnergy() noexcept;

  inline mtt::ObjectRef<HeaterObject>& heaterRef() noexcept;
  inline const mtt::ObjectRef<HeaterObject>& heaterRef() const noexcept;

signals:
  void gasSourceChanged(HeaterObject* newValue);
  void emittedEnergyChanged(float newValue);

protected:
  virtual void makeAction(float portion) override;

private:
  float _emittedEnergy;

  mtt::ObjectLink<HeaterObject,
                  HeatAction,
                  nullptr,
                  nullptr,
                  &HeatAction::gasSourceChanged> _heaterRef;
};

inline float HeatAction::emittedEnergy() const noexcept
{
  return _emittedEnergy;
}

inline void HeatAction::resetEmittedEnergy() noexcept
{
  setEmittedEnergy(0.f);
}

inline mtt::ObjectRef<HeaterObject>& HeatAction::heaterRef() noexcept
{
  return _heaterRef;
}

inline const mtt::ObjectRef<HeaterObject>&
                                        HeatAction::heaterRef() const noexcept
{
  return _heaterRef;
}
