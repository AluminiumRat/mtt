#pragma once

#include <Objects/AnimationAction.h>
#include <Objects/EmitterObject.h>

class ParticlesEmissionAction : public AnimationAction
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitParticlesEmissionAction,
                          visitConstParticlesEmissionAction,
                          AnimationAction)

  Q_PROPERTY( uint particlesNumber
              READ particlesNumber
              WRITE setParticlesNumber
              NOTIFY particlesNumberChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  ParticlesEmissionAction(const QString& name,
                          bool canBeRenamed,
                          const mtt::UID& id = mtt::UID());
  ParticlesEmissionAction(const ParticlesEmissionAction&) = delete;
  ParticlesEmissionAction& operator = (const ParticlesEmissionAction&) = delete;
  virtual ~ParticlesEmissionAction() noexcept = default;

  inline uint particlesNumber() const noexcept;
  void setParticlesNumber(uint newValue) noexcept;

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
                  ParticlesEmissionAction,
                  nullptr,
                  nullptr,
                  &ParticlesEmissionAction::emitterChanged> _emitterRef;
};

inline uint ParticlesEmissionAction::particlesNumber() const noexcept
{
  return _particlesNumber;
}

inline mtt::ObjectRef<EmitterObject>&
                                  ParticlesEmissionAction::emitterRef() noexcept
{
  return _emitterRef;
}

inline const mtt::ObjectRef<EmitterObject>&
                            ParticlesEmissionAction::emitterRef() const noexcept
{
  return _emitterRef;
}
