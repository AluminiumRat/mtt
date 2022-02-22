#pragma once

#include <mtt/application/Scene/ObjectGroup.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/editorLib/Objects/AnimationGroup.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Objects/EnvironmentGroup.h>
#include <mtt/editorLib/Objects/SkeletonGroup.h>

class RootObject : public mtt::Object
{
  Q_OBJECT

  //DEFINE_EXTENSION_ACCEPT(OEVisitorExtension,
  //                        visitRootObject,
  //                        visitConstRootObject,
  //                        mtt::Object)

public:
  RootObject( const QString& name,
              bool canBeRenamed,
              const mtt::UID& id = mtt::UID());
  RootObject(const RootObject&) = delete;
  RootObject& operator = (const RootObject&) = delete;
  virtual ~RootObject() noexcept = default;

  inline mtt::SkeletonGroup& skeletonGroup() noexcept;
  inline const mtt::SkeletonGroup& skeletonGroup() const noexcept;

  inline mtt::AnimationGroup& animationGroup() noexcept;
  inline const mtt::AnimationGroup& animationGroup() const noexcept;

  inline mtt::BackgroundObject& background() noexcept;
  inline const mtt::BackgroundObject& background() const noexcept;
  void changeBackground(std::unique_ptr<mtt::BackgroundObject> newBackground);

  inline mtt::EnvironmentGroup& environment() noexcept;
  inline const mtt::EnvironmentGroup& environment() const noexcept;

signals:
  void backgroundChanged(mtt::BackgroundObject& newBackground);

private:
  mtt::SkeletonGroup* _skeletonGroup;
  mtt::AnimationGroup* _animationGroup;
  mtt::BackgroundObject* _background;
  mtt::EnvironmentGroup* _environment;
};

inline mtt::SkeletonGroup& RootObject::skeletonGroup() noexcept
{
  return *_skeletonGroup;
}

inline const mtt::SkeletonGroup& RootObject::skeletonGroup() const noexcept
{
  return *_skeletonGroup;
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
