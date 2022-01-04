#pragma once

#include <Objects/MovableObject.h>

class EnvironmentObject : public MovableObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  EnvironmentObject(const QString& name,
                    bool canBeRenamed,
                    const mtt::UID = mtt::UID());
  EnvironmentObject(const EnvironmentObject&) = delete;
  EnvironmentObject& operator = (const EnvironmentObject&) = delete;
  virtual ~EnvironmentObject() noexcept = default;
};