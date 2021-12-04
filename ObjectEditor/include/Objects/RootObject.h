#pragma once

#include <mtt/Application/Scene/ObjectGroup.h>
#include <mtt/Application/Scene/Object.h>

#include <Objects/AnimationGroup.h>
#include <Objects/EnvironmentGroup.h>
#include <Objects/MaterialsGroup.h>
#include <Objects/SkeletonGroup.h>
#include <Objects/GeometryGroup.h>

class RootObject : public mtt::Object
{
  Q_OBJECT

public:
  RootObject( const QString& name,
              bool canBeRenamed,
              const mtt::UID& id = mtt::UID());
  RootObject(const RootObject&) = delete;
  RootObject& operator = (const RootObject&) = delete;
  virtual ~RootObject() noexcept = default;

  inline SkeletonGroup& skeletonGroup() noexcept;
  inline const SkeletonGroup& skeletonGroup() const noexcept;

  inline GeometryGroup& geometryGroup() noexcept;
  inline const GeometryGroup& geometryGroup() const noexcept;

  inline MaterialsGroup& materialsGroup() noexcept;
  inline const MaterialsGroup& materialsGroup() const noexcept;

  inline AnimationGroup& animationGroup() noexcept;
  inline const AnimationGroup& animationGroup() const noexcept;

  inline EnvironmentGroup& environment() noexcept;
  inline const EnvironmentGroup& environment() const noexcept;

private:
  SkeletonGroup* _skeletonGroup;
  GeometryGroup* _geometryGroup;
  MaterialsGroup* _materialsGroup;
  AnimationGroup* _animationGroup;
  EnvironmentGroup* _environment;
};

inline SkeletonGroup& RootObject::skeletonGroup() noexcept
{
  return *_skeletonGroup;
}

inline const SkeletonGroup& RootObject::skeletonGroup() const noexcept
{
  return *_skeletonGroup;
}

inline GeometryGroup& RootObject::geometryGroup() noexcept
{
  return *_geometryGroup;
}

inline const GeometryGroup& RootObject::geometryGroup() const noexcept
{
  return *_geometryGroup;
}

inline MaterialsGroup& RootObject::materialsGroup() noexcept
{
  return *_materialsGroup;
}

inline const MaterialsGroup& RootObject::materialsGroup() const noexcept
{
  return *_materialsGroup;
}

inline AnimationGroup& RootObject::animationGroup() noexcept
{
  return *_animationGroup;
}

inline const AnimationGroup& RootObject::animationGroup() const noexcept
{
  return *_animationGroup;
}

inline EnvironmentGroup& RootObject::environment() noexcept
{
  return *_environment;
}

inline const EnvironmentGroup& RootObject::environment() const noexcept
{
  return *_environment;
}
