#pragma once

#include <mtt/application/Scene/SpecialGroup.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>

namespace mtt
{
  class SkeletonGroup : public SpecialGroup<DisplayedObject,
                                            SkeletonObject>
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension)

  public:
    SkeletonGroup(const QString& name,
                  bool canBeRenamed,
                  const UID& id = UID());
    SkeletonGroup(const SkeletonGroup&) = delete;
    SkeletonGroup& operator = (const SkeletonGroup&) = delete;
    virtual ~SkeletonGroup() noexcept = default;

  signals:
    void childAdded(SkeletonObject& object);
    void childRemoved(SkeletonObject& object);
  };
}