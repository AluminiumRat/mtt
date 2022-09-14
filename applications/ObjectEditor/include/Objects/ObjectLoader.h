#pragma once

#include <mtt/editorLib/Objects/CEObjectLoader.h>

#include <Objects/OEVisitor.h>

class ObjectLoader : public OEVisitorT<mtt::CEObjectLoader>
{
public:
  ObjectLoader() = default;
  ObjectLoader(const ObjectLoader&) = delete;
  ObjectLoader& operator = (const ObjectLoader&) = delete;
  virtual ~ObjectLoader() noexcept = default;

  virtual void visitGeometryGroup(GeometryGroup& object) override;
  virtual void visitGeometryObject(GeometryObject& object) override;
  virtual void visitLODObject(LODObject& object) override;
  virtual void visitMaterialsGroup(MaterialsGroup& object) override;
  virtual void visitMaterialObject(MaterialObject& object) override;
  virtual void visitMeshObject(MeshObject& object) override;
  virtual void visitRootObject(RootObject& object) override;

protected:
  std::unique_ptr<BoneRefBatch> readBoneRefs();
  void readGeometry(mtt::CommonMeshGeometry& geometry);
};