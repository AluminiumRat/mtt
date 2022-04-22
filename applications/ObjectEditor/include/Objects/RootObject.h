#pragma once

#include <mtt/application/Scene/ObjectGroup.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/editorLib/Objects/AnimationGroup.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Objects/EnvironmentGroup.h>
#include <mtt/editorLib/Objects/SkeletonGroup.h>

#include <Objects/MaterialsGroup.h>
#include <Objects/GeometryGroup.h>

class RootObject : public mtt::Object
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension,
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

  inline mtt::SkeletonGroup& skeletonGroup() noexcept;
  inline const mtt::SkeletonGroup& skeletonGroup() const noexcept;

  inline GeometryGroup& geometryGroup() noexcept;
  inline const GeometryGroup& geometryGroup() const noexcept;

  inline MaterialsGroup& materialsGroup() noexcept;
  inline const MaterialsGroup& materialsGroup() const noexcept;

  inline mtt::AnimationGroup& animationGroup() noexcept;
  inline const mtt::AnimationGroup& animationGroup() const noexcept;

  void clear() noexcept;

private:
  mtt::SkeletonGroup* _skeletonGroup;
  GeometryGroup* _geometryGroup;
  MaterialsGroup* _materialsGroup;
  mtt::AnimationGroup* _animationGroup;
};

inline mtt::SkeletonGroup& RootObject::skeletonGroup() noexcept
{
  return *_skeletonGroup;
}

inline const mtt::SkeletonGroup& RootObject::skeletonGroup() const noexcept
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

inline mtt::AnimationGroup& RootObject::animationGroup() noexcept
{
  return *_animationGroup;
}

inline const mtt::AnimationGroup& RootObject::animationGroup() const noexcept
{
  return *_animationGroup;
}
