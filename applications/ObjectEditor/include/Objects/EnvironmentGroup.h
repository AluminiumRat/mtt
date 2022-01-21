#pragma once

#include <mtt/application/Scene/SpecialGroup.h>

#include <Objects/EnvironmentObject.h>

class EnvironmentGroup : public mtt::SpecialGroup<DisplayedObject,
                                                  EnvironmentObject>
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  EnvironmentGroup( const QString& name,
                    bool canBeRenamed,
                    const mtt::UID& id = mtt::UID());
  EnvironmentGroup(const EnvironmentGroup&) = delete;
  EnvironmentGroup& operator = (const EnvironmentGroup&) = delete;
  virtual ~EnvironmentGroup() noexcept = default;

signals:
  void childAdded(EnvironmentObject& object);
  void childRemoved(EnvironmentObject& object);
};
