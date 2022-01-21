#pragma once

#include <mtt/application/Scene/SpecialGroup.h>

#include <Objects/MaterialObject.h>

class MaterialsGroup : public mtt::SpecialGroup<mtt::Object,
                                                MaterialObject>
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  inline MaterialsGroup(const QString& name,
                        bool canBeRenamed,
                        const mtt::UID& id = mtt::UID());
  MaterialsGroup(const MaterialsGroup&) = delete;
  MaterialsGroup& operator = (const MaterialsGroup&) = delete;
  virtual ~MaterialsGroup() noexcept = default;

signals:
  void childAdded(MaterialObject& object);
  void childRemoved(MaterialObject& object);
};

inline MaterialsGroup::MaterialsGroup(const QString& name,
                                      bool canBeRenamed,
                                      const mtt::UID& id) :
  SpecialGroup(name, canBeRenamed, id)
{
}