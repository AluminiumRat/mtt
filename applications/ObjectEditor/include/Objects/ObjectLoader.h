#pragma once

#include <mtt/editorLib/Objects/ObjectLoader.h>

#include <Objects/OEVisitor.h>

class ObjectLoader : public OEVisitorT<mtt::ObjectLoader>
{
public:
  ObjectLoader() = default;
  ObjectLoader(const ObjectLoader&) = delete;
  ObjectLoader& operator = (const ObjectLoader&) = delete;
  virtual ~ObjectLoader() noexcept = default;

  virtual void visitGeometryObject(GeometryObject& object) override;
  virtual void visitLODObject(LODObject& object) override;
  virtual void visitMaterialObject(MaterialObject& object) override;
  virtual void visitMeshObject(MeshObject& object) override;

protected:
  std::unique_ptr<BoneRefBatch> readBoneRefs();
  void readGeometry(mtt::CommonMeshGeometry& geometry);
};