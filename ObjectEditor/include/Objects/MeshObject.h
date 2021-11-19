#pragma once

#include <mtt/Utilities/CommonMeshGeometry.h>

#include <Objects/BoneRefBatch.h>
#include <Objects/GeometryObject.h>
#include <Objects/MaterialObject.h>

class MeshObject : public GeometryObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  explicit MeshObject(const mtt::UID& id = mtt::UID());
  MeshObject(const MeshObject&) = delete;
  MeshObject& operator = (const MeshObject&) = delete;
  virtual ~MeshObject() noexcept = default;

  inline const mtt::CommonMeshGeometry& geometry() const noexcept;
  void setGeometry(mtt::CommonMeshGeometry&& newGeometry) noexcept;

  inline BoneRefBatch& boneRefs() noexcept;
  inline const BoneRefBatch& boneRefs() const noexcept;
  /// refs object should not be nullptr
  void setBoneRefs(std::unique_ptr<BoneRefBatch> refs);

  inline MaterialObject& material() noexcept;
  inline const MaterialObject& material() const noexcept;

signals:
  void geometryChanged();
  void boneRefsChanged(BoneRefBatch& newRefs);

private:
  mtt::CommonMeshGeometry _geometry;
  BoneRefBatch* _boneRefs;
  MaterialObject* _material;
};

inline const mtt::CommonMeshGeometry& MeshObject::geometry() const noexcept
{
  return _geometry;
}

inline BoneRefBatch& MeshObject::boneRefs() noexcept
{
  return *_boneRefs;
}

inline const BoneRefBatch& MeshObject::boneRefs() const noexcept
{
  return *_boneRefs;
}

inline MaterialObject& MeshObject::material() noexcept
{
  return *_material;
}

inline const MaterialObject& MeshObject::material() const noexcept
{
  return *_material;
}
