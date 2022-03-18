#pragma once

#include <mtt/application/Scene/SpecialGroup.h>

#include <Objects/HierarhicalObject.h>

class ModificatorGroup : public mtt::SpecialGroup<mtt::DisplayedObject,
                                                  HierarhicalObject>
{
  Q_OBJECT
  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitModificatorGroup,
                          visitConstModificatorGroup,
                          mtt::DisplayedObject)

public:
  ModificatorGroup(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  ModificatorGroup(const ModificatorGroup&) = delete;
  ModificatorGroup& operator = (const ModificatorGroup&) = delete;
  virtual ~ModificatorGroup() noexcept = default;

signals:
  void childAdded(HierarhicalObject& object);
  void childRemoved(HierarhicalObject& object);
};
