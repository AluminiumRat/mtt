#pragma once

#include <mtt/editorLib/Objects/ScalableObject.h>

#include <Objects/PEVisitorExtension.h>

class HierarhicalObject : public mtt::ScalableObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitHierarhicalObject,
                          visitConstHierarhicalObject,
                          mtt::MovableObject)

public:
  HierarhicalObject(const QString& name,
                    bool canBeRenamed,
                    const mtt::UID& id = mtt::UID());
  HierarhicalObject(const HierarhicalObject&) = delete;
  HierarhicalObject& operator = (const HierarhicalObject&) = delete;
  virtual ~HierarhicalObject() noexcept = default;
};
