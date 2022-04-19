#pragma once

#include <Objects/Fluid/HeaterObject.h>
#include <Objects/AnimationAction.h>

class HeatingAction : public AnimationAction
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitHeatingAction,
                          visitConstHeatingAction,
                          AnimationAction)

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
  HeatingAction(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  HeatingAction(const HeatingAction&) = delete;
  HeatingAction& operator = (const HeatingAction&) = delete;
  virtual ~HeatingAction() noexcept = default;

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
                  HeatingAction,
                  nullptr,
                  nullptr,
                  &HeatingAction::gasSourceChanged> _heaterRef;
};

inline float HeatingAction::emittedEnergy() const noexcept
{
  return _emittedEnergy;
}

inline void HeatingAction::resetEmittedEnergy() noexcept
{
  setEmittedEnergy(0.f);
}

inline mtt::ObjectRef<HeaterObject>& HeatingAction::heaterRef() noexcept
{
  return _heaterRef;
}

inline const mtt::ObjectRef<HeaterObject>&
                                      HeatingAction::heaterRef() const noexcept
{
  return _heaterRef;
}
