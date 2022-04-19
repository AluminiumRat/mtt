#pragma once

#include <Objects/Fluid/GasSource.h>
#include <Objects/AnimationAction.h>

class GasEmissionAction : public AnimationAction
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitGasEmissionAction,
                          visitConstGasEmissionAction,
                          AnimationAction)

  Q_PROPERTY( uint emittedVolume
              READ emittedVolume
              WRITE setEmittedVolume
              RESET resetEmittedVolume
              NOTIFY emittedVolumeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  GasEmissionAction(const QString& name,
                    bool canBeRenamed,
                    const mtt::UID& id = mtt::UID());
  GasEmissionAction(const GasEmissionAction&) = delete;
  GasEmissionAction& operator = (const GasEmissionAction&) = delete;
  virtual ~GasEmissionAction() noexcept = default;

  inline float emittedVolume() const noexcept;
  void setEmittedVolume(float newValue) noexcept;
  inline void resetEmittedVolume() noexcept;

  inline mtt::ObjectRef<GasSource>& gasSourceRef() noexcept;
  inline const mtt::ObjectRef<GasSource>& gasSourceRef() const noexcept;

signals:
  void gasSourceChanged(GasSource* newValue);
  void emittedVolumeChanged(float newValue);

protected:
  virtual void makeAction(float portion) override;

private:
  float _emittedVolume;

  mtt::ObjectLink<GasSource,
                  GasEmissionAction,
                  nullptr,
                  nullptr,
                  &GasEmissionAction::gasSourceChanged> _gasSourceRef;
};

inline float GasEmissionAction::emittedVolume() const noexcept
{
  return _emittedVolume;
}

inline void GasEmissionAction::resetEmittedVolume() noexcept
{
  setEmittedVolume(0.f);
}

inline mtt::ObjectRef<GasSource>& GasEmissionAction::gasSourceRef() noexcept
{
  return _gasSourceRef;
}

inline const mtt::ObjectRef<GasSource>&
                                GasEmissionAction::gasSourceRef() const noexcept
{
  return _gasSourceRef;
}
