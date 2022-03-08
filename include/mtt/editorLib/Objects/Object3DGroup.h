#pragma once

#include <mtt/editorLib/Objects/Object3D.h>
#include <mtt/application/Scene/SpecialGroup.h>

namespace mtt
{
  class Object3DGroup : public SpecialGroup<DisplayedObject,
                                            Object3D>
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitObject3DGroup,
                            visitConstObject3DGroup,
                            DisplayedObject)

  public:
    Object3DGroup(const QString& name,
                  bool canBeRenamed,
                  const UID& id = UID());
    Object3DGroup(const Object3DGroup&) = delete;
    Object3DGroup& operator = (const Object3DGroup&) = delete;
    virtual ~Object3DGroup() noexcept = default;

  signals:
    void childAdded(Object3D& object);
    void childRemoved(Object3D& object);
  };
}