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

  inline ParticleField& particleField() noexcept;
  inline const ParticleField& particleField() const noexcept;

private:
  ModificatorGroup* _modificatorsGroup;
  ParticleAnimation* _animation;
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

inline ParticleField& RootObject::particleField() noexcept
{
  return *_particleField;
}

inline const ParticleField& RootObject::particleField() const noexcept
{
  return *_particleField;
}
