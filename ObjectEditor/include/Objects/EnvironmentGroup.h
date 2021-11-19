#pragma once

#include <mtt/Application/Scene/SpecialGroup.h>

#include <Objects/BackgroundObject.h>
#include <Objects/EnvironmentObject.h>

class EnvironmentGroup : public mtt::SpecialGroup<DisplayedObject,
                                                  EnvironmentObject>
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  explicit EnvironmentGroup(const mtt::UID& id = mtt::UID());
  EnvironmentGroup(const EnvironmentGroup&) = delete;
  EnvironmentGroup& operator = (const EnvironmentGroup&) = delete;
  virtual ~EnvironmentGroup() noexcept = default;

public:
  inline BackgroundObject& background() noexcept;
  inline const BackgroundObject& background() const noexcept;

signals:
  void childAdded(EnvironmentObject& object);
  void childRemoved(EnvironmentObject& object);

private:
  BackgroundObject* _background;
};

inline BackgroundObject& EnvironmentGroup::background() noexcept
{
  return *_background;
}

inline const BackgroundObject& EnvironmentGroup::background() const noexcept
{
  return *_background;
}
