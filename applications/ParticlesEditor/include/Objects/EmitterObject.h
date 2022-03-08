#pragma once

#include <Objects/ModificatorObject.h>

class EmitterObject : public ModificatorObject
{
  Q_OBJECT
  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitEmitterObject,
                          visitConstEmitterObject,
                          ModificatorObject)

public:
  EmitterObject(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  EmitterObject(const EmitterObject&) = delete;
  EmitterObject& operator = (const EmitterObject&) = delete;
  virtual ~EmitterObject() noexcept = default;
};