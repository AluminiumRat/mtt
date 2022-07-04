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

  Q_PROPERTY( mtt::TimeT duration
              READ duration
              WRITE setDuration
              NOTIFY durationChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  ParticleAnimation(const QString& name,
                    bool canBeRenamed,
                    const mtt::UID& id = mtt::UID());
  ParticleAnimation(const ParticleAnimation&) = delete;
  ParticleAnimation& operator = (const ParticleAnimation&) = delete;
  virtual ~ParticleAnimation() noexcept = default;

  inline mtt::TimeT duration() const noexcept;
  void setDuration(mtt::TimeT newValue) noexcept;

  inline mtt::ObjectRef<ParticleField>& fieldRef() noexcept;
  inline const mtt::ObjectRef<ParticleField>& fieldRef() const noexcept;

  virtual void update(mtt::TimeRange time) override;

signals:
  void durationChanged(mtt::TimeT newValue);
  void fieldChanged(ParticleField* newField);

protected:
  virtual mtt::TimeRange calculateTiming() const noexcept override;

private:
  mtt::TimeT _duration;

  mtt::ObjectLink<ParticleField,
                  ParticleAnimation,
                  nullptr,
                  nullptr,
                  &ParticleAnimation::fieldChanged> _fieldRef;
};

inline mtt::TimeT ParticleAnimation::duration() const noexcept
{
  return _duration;
}

inline mtt::ObjectRef<ParticleField>& ParticleAnimation::fieldRef() noexcept
{
  return _fieldRef;
}

inline const mtt::ObjectRef<ParticleField>&
                                    ParticleAnimation::fieldRef() const noexcept
{
  return _fieldRef;
}
