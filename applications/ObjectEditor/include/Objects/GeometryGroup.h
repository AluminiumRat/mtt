#pragma once

#include <mtt/application/Scene/SpecialGroup.h>

#include <Objects/LODObject.h>

class GeometryGroup : public mtt::SpecialGroup< mtt::DisplayedObject,
                                                LODObject>
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  GeometryGroup(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  GeometryGroup(const GeometryGroup&) = delete;
  GeometryGroup& operator = (const GeometryGroup&) = delete;
  virtual ~GeometryGroup() noexcept = default;

signals:
  void childAdded(LODObject& object);
  void childRemoved(LODObject& object);
};
