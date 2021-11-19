#pragma once

#include <mtt/Core/Scene/SpecialGroup.h>

#include <Objects/LODObject.h>

class GeometryGroup : public mtt::SpecialGroup< DisplayedObject,
                                                LODObject>
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  explicit GeometryGroup(const mtt::UID& id = mtt::UID());
  GeometryGroup(const GeometryGroup&) = delete;
  GeometryGroup& operator = (const GeometryGroup&) = delete;
  virtual ~GeometryGroup() noexcept = default;

signals:
  void childAdded(LODObject& object);
  void childRemoved(LODObject& object);
};
