#pragma once

#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/utilities/CommonMeshGeometry.h>

#include <Objects/BoneRefBatch.h>
#include <Objects/GeometryObject.h>
#include <Objects/MaterialObject.h>

class MeshObject : public GeometryObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension,
                          visitMeshObject,
                          visitConstMeshObject,
                          GeometryObject)

public:
  MeshObject( const QString& name,
              bool canBeRenamed,
              const mtt::UID& id = mtt::UID());
  MeshObject(const MeshObject&) = delete;
  MeshObject& operator = (const MeshObject&) = delete;
  virtual ~MeshObject() noexcept = default;

  inline const mtt::CommonMeshGeometry& geometry() const noexcept;
  void setGeometry(mtt::CommonMeshGeometry&& newGeometry) noexcept;

  inline BoneRefBatch& boneRefs() noexcept;
  inline const BoneRefBatch& boneRefs() const noexcept;
  /// refs object should not be nullptr
  void setBoneRefs(std::unique_ptr<BoneRefBatch> refs);

  inline const mtt::ObjectRef<MaterialObject>& materialRef() const noexcept;
  inline MaterialObject* material() const noexcept;
  /// You can use nullptr to remove link
  void setMaterial(MaterialObject* material);
  /// You can use invalid UID to remove link
  void setMaterialId(const mtt::UID& id);

signals:
  void geometryChanged();
  void boneRefsChanged(BoneRefBatch& newRefs);
  void materialRefChanged(MaterialObject* material);

private:
  void _connectMaterial(MaterialObject& material);
  void _disconnectMaterial(MaterialObject& material) noexcept;

private:
  mtt::CommonMeshGeometry _geometry;

  using MaterialLink = mtt::ObjectLink< MaterialObject,
                                        MeshObject,
                                        &MeshObject::_connectMaterial,
                                        &MeshObject::_disconnectMaterial>;
  MaterialLink _materialLink;

  BoneRefBatch* _boneRefs;
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

inline const mtt::ObjectRef<MaterialObject>&
                                      MeshObject::materialRef() const noexcept
{
  return _materialLink;
}

inline MaterialObject* MeshObject::material() const noexcept
{
  return _materialLink.get();
}
