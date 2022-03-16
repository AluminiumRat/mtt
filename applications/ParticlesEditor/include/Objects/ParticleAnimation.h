#pragma once

#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/editorLib/Objects/AnimationObject.h>

#include <Objects/ParticleField.h>
#include <Objects/PEVisitorExtension.h>

class ParticleAnimation : public mtt::AnimationObject
{
  Q_OBJECT
  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitParticleAnimation,
                          visitConstParticleAnimation,
                          mtt::AnimationObject)

public:
  ParticleAnimation(const QString& name,
                    bool canBeRenamed,
                    const mtt::UID& id = mtt::UID());
  ParticleAnimation(const ParticleAnimation&) = delete;
  ParticleAnimation& operator = (const ParticleAnimation&) = delete;
  virtual ~ParticleAnimation() noexcept = default;

  inline mtt::ObjectRef<ParticleField>& fieldRef() noexcept;
  inline const mtt::ObjectRef<ParticleField>& fieldRef() const noexcept;

  void setTimeRange(const mtt::Range<TimeType>& newValue) noexcept;
  inline void resetTimeRange() noexcept;

  virtual void update(TimeType time) override;

signals:
  void fieldChanged(ParticleField* newField);

protected:
  virtual mtt::Range<TimeType> calculateTiming() const noexcept override;

private:
  mtt::ObjectLink<ParticleField,
                  ParticleAnimation,
                  nullptr,
                  nullptr,
                  &ParticleAnimation::fieldChanged> _fieldRef;

  mtt::Range<TimeType> _storedTimeRange;
  TimeType _lastTime;
};

inline mtt::ObjectRef<ParticleField>& ParticleAnimation::fieldRef() noexcept
{
  return _fieldRef;
}

inline const mtt::ObjectRef<ParticleField>&
                                    ParticleAnimation::fieldRef() const noexcept
{
  return _fieldRef;
}

inline void ParticleAnimation::resetTimeRange() noexcept
{
  setTimeRange(mtt::Range<TimeType>());
}
