#pragma once

#include <mtt/application/Scene/ObjectGroup.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Objects/EnvironmentGroup.h>

#include <Objects/ModificatorGroup.h>
#include <Objects/ParticleAnimation.h>
#include <Objects/ParticleField.h>
#include <Objects/PEVisitorExtension.h>

class RootObject : public mtt::Object
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitRootObject,
                          visitConstRootObject,
                          mtt::Object)

public:
  RootObject( const QString& name,
              bool canBeRenamed,
              const mtt::UID& id = mtt::UID());
  RootObject(const RootObject&) = delete;
  RootObject& operator = (const RootObject&) = delete;
  virtual ~RootObject() noexcept = default;

  inline ModificatorGroup& modificatorsGroup() noexcept;
  inline const ModificatorGroup& modificatorsGroup() const noexcept;

  inline ParticleAnimation& animation() noexcept;
  inline const ParticleAnimation& animation() const noexcept;
  /// newAnimation must be not null
  void changeAnimation(std::unique_ptr<ParticleAnimation> newAnimation);

  inline mtt::BackgroundObject& background() noexcept;
  inline const mtt::BackgroundObject& background() const noexcept;
  /// newBackground must be not null
  void changeBackground(std::unique_ptr<mtt::BackgroundObject> newBackground);

  inline mtt::EnvironmentGroup& environment() noexcept;
  inline const mtt::EnvironmentGroup& environment() const noexcept;

  inline ParticleField& particleField() noexcept;
  inline const ParticleField& particleField() const noexcept;
  /// newField must be not null
  void changeParticleField(std::unique_ptr<ParticleField> newField);

signals:
  void animationChanged(ParticleAnimation& newAnimation);
  void backgroundChanged(mtt::BackgroundObject& newBackground);
  void particleFieldChanged(ParticleField& newField);

private:
  ModificatorGroup* _modificatorsGroup;
  ParticleAnimation* _animation;
  mtt::BackgroundObject* _background;
  mtt::EnvironmentGroup* _environment;
  ParticleField* _particleField;
};

inline ModificatorGroup& RootObject::modificatorsGroup() noexcept
{
  return *_modificatorsGroup;
}

inline const ModificatorGroup& RootObject::modificatorsGroup() const noexcept
{
  return *_modificatorsGroup;
}

inline ParticleAnimation& RootObject::animation() noexcept
{
  return *_animation;
}

inline const ParticleAnimation& RootObject::animation() const noexcept
{
  return *_animation;
}

inline mtt::BackgroundObject& RootObject::background() noexcept
{
  return *_background;
}

inline const mtt::BackgroundObject& RootObject::background() const noexcept
{
  return *_background;
}

inline mtt::EnvironmentGroup& RootObject::environment() noexcept
{
  return *_environment;
}

inline const mtt::EnvironmentGroup& RootObject::environment() const noexcept
{
  return *_environment;
}

inline ParticleField& RootObject::particleField() noexcept
{
  return *_particleField;
}

inline const ParticleField& RootObject::particleField() const noexcept
{
  return *_particleField;
}
