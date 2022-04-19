#pragma once

#include <Objects/Fluid/GasSource.h>
#include <Objects/ActionAnimationTrack.h>

class EmitGasAction : public ActionAnimationTrack
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitEmitGasAction,
                          visitConstEmitGasAction,
                          ActionAnimationTrack)

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
  EmitGasAction(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  EmitGasAction(const EmitGasAction&) = delete;
  EmitGasAction& operator = (const EmitGasAction&) = delete;
  virtual ~EmitGasAction() noexcept = default;

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
                  EmitGasAction,
                  nullptr,
                  nullptr,
                  &EmitGasAction::gasSourceChanged> _gasSourceRef;
};

inline float EmitGasAction::emittedVolume() const noexcept
{
  return _emittedVolume;
}

inline void EmitGasAction::resetEmittedVolume() noexcept
{
  setEmittedVolume(0.f);
}

inline mtt::ObjectRef<GasSource>& EmitGasAction::gasSourceRef() noexcept
{
  return _gasSourceRef;
}

inline const mtt::ObjectRef<GasSource>&
                                EmitGasAction::gasSourceRef() const noexcept
{
  return _gasSourceRef;
}
