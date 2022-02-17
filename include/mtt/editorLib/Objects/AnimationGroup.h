#pragma once

#include <mtt/application/Scene/SpecialGroup.h>
#include <mtt/editorLib/Objects/AnimationObject.h>

namespace mtt
{
  class AnimationGroup : public SpecialGroup<Object, AnimationObject>
  {
    Q_OBJECT

    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitAnimationGroup,
                            visitConstAnimationGroup,
                            Object)

  public:
    explicit AnimationGroup(const QString& name,
                            bool canBeRenamed,
                            const UID& id = UID());
    AnimationGroup(const AnimationGroup&) = delete;
    AnimationGroup& operator = (const AnimationGroup&) = delete;
    virtual ~AnimationGroup() noexcept = default;

  signals:
    void childAdded(AnimationObject& object);
    void childRemoved(AnimationObject& object);
  };
}