#pragma once

#include <Objects/MovableObject.h>

class EnvironmentObject : public MovableObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  explicit EnvironmentObject(const mtt::UID = mtt::UID());
  EnvironmentObject(const EnvironmentObject&) = delete;
  EnvironmentObject& operator = (const EnvironmentObject&) = delete;
  virtual ~EnvironmentObject() noexcept = default;
};