#pragma once

#include <mtt/application/Scene/SpecialGroup.h>

#include <Objects/AnimationObject.h>

class AnimationGroup : public mtt::SpecialGroup<mtt::Object,
                                                AnimationObject>
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  explicit AnimationGroup(const QString& name,
                          bool canBeRenamed,
                          const mtt::UID& id = mtt::UID());
  AnimationGroup(const AnimationGroup&) = delete;
  AnimationGroup& operator = (const AnimationGroup&) = delete;
  virtual ~AnimationGroup() noexcept = default;

signals:
  void childAdded(AnimationObject& object);
  void childRemoved(AnimationObject& object);
};
