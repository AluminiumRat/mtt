#pragma once

#include <mtt/editorLib/Objects/ObjectSaver.h>

#include <Objects/OEVisitor.h>

class ObjectSaver : public OEVisitorT<mtt::ObjectSaver>
{
public:
  ObjectSaver() = default;
  ObjectSaver(const ObjectSaver&) = delete;
  ObjectSaver& operator = (const ObjectSaver&) = delete;
  virtual ~ObjectSaver() noexcept = default;

  virtual void visitConstGeometryGroup(const GeometryGroup& object) override;
  virtual void visitConstGeometryObject(const GeometryObject& object) override;
  virtual void visitConstLODObject(const LODObject& object) override;
  virtual void visitConstMaterialsGroup(const MaterialsGroup& object) override;
  virtual void visitConstMaterialObject(const MaterialObject& object) override;
  virtual void visitConstMeshObject(const MeshObject& object) override;
  virtual void visitConstRootObject(const RootObject& object) override;

protected:
  void saveGeometry(const mtt::CommonMeshGeometry& geometry);
  void saveBoneRefs(const BoneRefBatch& refs);
};