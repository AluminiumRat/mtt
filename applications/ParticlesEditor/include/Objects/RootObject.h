#pragma once

#include <mtt/application/Scene/ObjectGroup.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/editorLib/Objects/AnimationGroup.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Objects/EnvironmentGroup.h>

#include <Objects/ModificatorGroup.h>
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

  inline mtt::AnimationGroup& animationGroup() noexcept;
  inline const mtt::AnimationGroup& animationGroup() const noexcept;

  inline mtt::BackgroundObject& background() noexcept;
  inline const mtt::BackgroundObject& background() const noexcept;
  void changeBackground(std::unique_ptr<mtt::BackgroundObject> newBackground);

  inline mtt::EnvironmentGroup& environment() noexcept;
  inline const mtt::EnvironmentGroup& environment() const noexcept;

  inline ParticleField& particleField() noexcept;
  inline const ParticleField& particleField() const noexcept;

signals:
  void backgroundChanged(mtt::BackgroundObject& newBackground);

private:
  ModificatorGroup* _modificatorsGroup;
  mtt::AnimationGroup* _animationGroup;
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

inline mtt::AnimationGroup& RootObject::animationGroup() noexcept
{
  return *_animationGroup;
}

inline const mtt::AnimationGroup& RootObject::animationGroup() const noexcept
{
  return *_animationGroup;
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
