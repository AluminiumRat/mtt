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
  virtual void visit(const BoneRefBatch& object) = 0;
  virtual void visit(BoneRefBatch& object) = 0;

  virtual void visit(const BoneRefObject& object) = 0;
  virtual void visit(BoneRefObject& object) = 0;

  virtual void visit(const GeometryGroup& object) = 0;
  virtual void visit(GeometryGroup& object) = 0;

  virtual void visit(const GeometryObject& object) = 0;
  virtual void visit(GeometryObject& object) = 0;

  virtual void visit(const LODObject& object) = 0;
  virtual void visit(LODObject& object) = 0;

  virtual void visit(const MaterialObject& object) = 0;
  virtual void visit(MaterialObject& object) = 0;

  virtual void visit(const MaterialsGroup& object) = 0;
  virtual void visit(MaterialsGroup& object) = 0;

  virtual void visit(const MeshObject& object) = 0;
  virtual void visit(MeshObject& object) = 0;

  virtual void visit(const RootObject& object) = 0;
  virtual void visit(RootObject& object) = 0;
};