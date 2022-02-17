#pragma once

#include <mtt/application/Scene/ObjectVisitor.h>

class BoneRefBatch;
class BoneRefObject;
class GeometryGroup;
class GeometryObject;
class LODObject;
class MaterialObject;
class MaterialsGroup;
class MeshObject;
class RootObject;

class OEVisitorExtension
{
public:
  static mtt::ObjectVisitor::ExtensionID extensionID;

public:
  virtual void visitConstBoneRefBatch(const BoneRefBatch& object) = 0;
  virtual void visitBoneRefBatch(BoneRefBatch& object) = 0;

  virtual void visitConstBoneRefObject(const BoneRefObject& object) = 0;
  virtual void visitBoneRefObject(BoneRefObject& object) = 0;

  virtual void visitConstGeometryGroup(const GeometryGroup& object) = 0;
  virtual void visitGeometryGroup(GeometryGroup& object) = 0;

  virtual void visitConstGeometryObject(const GeometryObject& object) = 0;
  virtual void visitGeometryObject(GeometryObject& object) = 0;

  virtual void visitConstLODObject(const LODObject& object) = 0;
  virtual void visitLODObject(LODObject& object) = 0;

  virtual void visitConstMaterialObject(const MaterialObject& object) = 0;
  virtual void visitMaterialObject(MaterialObject& object) = 0;

  virtual void visitConstMaterialsGroup(const MaterialsGroup& object) = 0;
  virtual void visitMaterialsGroup(MaterialsGroup& object) = 0;

  virtual void visitConstMeshObject(const MeshObject& object) = 0;
  virtual void visitMeshObject(MeshObject& object) = 0;

  virtual void visitConstRootObject(const RootObject& object) = 0;
  virtual void visitRootObject(RootObject& object) = 0;
};