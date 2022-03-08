#pragma once

#include <mtt/editorLib/Objects/ScalableObject.h>

#include <Objects/PEVisitorExtension.h>

class ModificatorObject : public mtt::ScalableObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitModificatorObject,
                          visitConstModificatorObject,
                          mtt::MovableObject)

public:
  ModificatorObject(const QString& name,
                    bool canBeRenamed,
                    const mtt::UID& id = mtt::UID());
  ModificatorObject(const ModificatorObject&) = delete;
  ModificatorObject& operator = (const ModificatorObject&) = delete;
  virtual ~ModificatorObject() noexcept = default;
};