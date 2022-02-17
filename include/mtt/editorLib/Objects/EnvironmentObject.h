#pragma once

#include <mtt/editorLib/Objects/MovableObject.h>

namespace mtt
{
  class EnvironmentObject : public MovableObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitEnvironmentObject,
                            visitConstEnvironmentObject,
                            MovableObject)

  public:
    EnvironmentObject(const QString& name,
                      bool canBeRenamed,
                      const UID = UID());
    EnvironmentObject(const EnvironmentObject&) = delete;
    EnvironmentObject& operator = (const EnvironmentObject&) = delete;
    virtual ~EnvironmentObject() noexcept = default;
  };
}