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

  inline mtt::ObjectRef<MaterialObject>& materialRef() noexcept;
  inline const mtt::ObjectRef<MaterialObject>& materialRef() const noexcept;

signals:
  void geometryChanged();
  void boneRefsChanged(BoneRefBatch& newRefs);
  void materialRefChanged(MaterialObject* material);

private:
  mtt::CommonMeshGeometry _geometry;

  using MaterialLink = mtt::ObjectLink< MaterialObject,
                                        MeshObject,
                                        nullptr,
                                        nullptr,
                                        &MeshObject::materialRefChanged>;
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

inline mtt::ObjectRef<MaterialObject>& MeshObject::materialRef() noexcept
{
  return _materialLink;
}

inline const mtt::ObjectRef<MaterialObject>&
                                      MeshObject::materialRef() const noexcept
{
  return _materialLink;
}
