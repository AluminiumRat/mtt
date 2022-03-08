#pragma once

#include <mtt/application/Scene/SpecialGroup.h>

#include <Objects/ModificatorObject.h>

class ModificatorGroup : public mtt::SpecialGroup<mtt::DisplayedObject,
                                                  ModificatorObject>
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
  void childAdded(ModificatorObject& object);
  void childRemoved(ModificatorObject& object);
};
