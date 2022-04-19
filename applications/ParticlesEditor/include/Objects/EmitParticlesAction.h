#pragma once

#include <Objects/ActionAnimationTrack.h>
#include <Objects/EmitterObject.h>

class EmitParticlesAction : public ActionAnimationTrack
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitEmitParticlesAction,
                          visitConstEmitParticlesAction,
                          ActionAnimationTrack)

  Q_PROPERTY( uint particlesNumber
              READ particlesNumber
              WRITE setParticlesNumber
              RESET resetParticlesNumber
              NOTIFY particlesNumberChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  EmitParticlesAction(const QString& name,
                      bool canBeRenamed,
                      const mtt::UID& id = mtt::UID());
  EmitParticlesAction(const EmitParticlesAction&) = delete;
  EmitParticlesAction& operator = (const EmitParticlesAction&) = delete;
  virtual ~EmitParticlesAction() noexcept = default;

  inline uint particlesNumber() const noexcept;
  void setParticlesNumber(uint newValue) noexcept;
  inline void resetParticlesNumber() noexcept;

  inline mtt::ObjectRef<EmitterObject>& emitterRef() noexcept;
  inline const mtt::ObjectRef<EmitterObject>& emitterRef() const noexcept;

signals:
  void emitterChanged(EmitterObject* newValue);
  void particlesNumberChanged(uint newValue);

protected:
  virtual void makeAction(float portion) override;

private:
  uint _particlesNumber;

  mtt::ObjectLink<EmitterObject,
                  EmitParticlesAction,
                  nullptr,
                  nullptr,
                  &EmitParticlesAction::emitterChanged> _emitterRef;
};

inline uint EmitParticlesAction::particlesNumber() const noexcept
{
  return _particlesNumber;
}

inline void EmitParticlesAction::resetParticlesNumber() noexcept
{
  setParticlesNumber(0);
}

inline mtt::ObjectRef<EmitterObject>& EmitParticlesAction::emitterRef() noexcept
{
  return _emitterRef;
}

inline const mtt::ObjectRef<EmitterObject>&
                                EmitParticlesAction::emitterRef() const noexcept
{
  return _emitterRef;
}
