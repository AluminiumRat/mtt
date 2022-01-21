#pragma once

#include <mtt/application/Scene/SpecialGroup.h>

#include <Objects/SkeletonObject.h>

class SkeletonGroup : public mtt::SpecialGroup< DisplayedObject,
                                                SkeletonObject>
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  SkeletonGroup(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  SkeletonGroup(const SkeletonGroup&) = delete;
  SkeletonGroup& operator = (const SkeletonGroup&) = delete;
  virtual ~SkeletonGroup() noexcept = default;

signals:
  void childAdded(SkeletonObject& object);
  void childRemoved(SkeletonObject& object);
};
