#pragma once

#include <mtt/Application/Scene/ObjectVisitor.h>

class AmbientLightObject;
class AnimationGroup;
class AnimationObject;
class AnimationTrack;
class BackgroundObject;
class BoneRefBatch;
class BoneRefObject;
class CubemapObject;
class DirectLightObject;
class DisplayedObject;
class EnvironmentGroup;
class GeometryGroup;
class EnvironmentObject;
class GeometryObject;
class LightObject;
class LODObject;
class MaterialObject;
class MeshObject;
class MovableObject;
class Object3D;
class RootObject;
class RotatableObject;
class ScalableObject;
class SkeletonGroup;
class SkeletonObject;

class OEVisitorExtension
{
public:
  static mtt::ObjectVisitor::ExtensionID extensionID;

public:
  virtual void visit(const AmbientLightObject& object) = 0;
  virtual void visit(AmbientLightObject& object) = 0;

  virtual void visit(const AnimationGroup& object) = 0;
  virtual void visit(AnimationGroup& object) = 0;

  virtual void visit(const AnimationObject& object) = 0;
  virtual void visit(AnimationObject& object) = 0;

  virtual void visit(const AnimationTrack& object) = 0;
  virtual void visit(AnimationTrack& object) = 0;

  virtual void visit(const BackgroundObject& object) = 0;
  virtual void visit(BackgroundObject& object) = 0;

  virtual void visit(const BoneRefBatch& object) = 0;
  virtual void visit(BoneRefBatch& object) = 0;

  virtual void visit(const BoneRefObject& object) = 0;
  virtual void visit(BoneRefObject& object) = 0;

  virtual void visit(const CubemapObject& object) = 0;
  virtual void visit(CubemapObject& object) = 0;

  virtual void visit(const DirectLightObject& object) = 0;
  virtual void visit(DirectLightObject& object) = 0;

  virtual void visit(const DisplayedObject& object) = 0;
  virtual void visit(DisplayedObject& object) = 0;

  virtual void visit(const EnvironmentGroup& object) = 0;
  virtual void visit(EnvironmentGroup& object) = 0;

  virtual void visit(const EnvironmentObject& object) = 0;
  virtual void visit(EnvironmentObject& object) = 0;

  virtual void visit(const GeometryGroup& object) = 0;
  virtual void visit(GeometryGroup& object) = 0;

  virtual void visit(const GeometryObject& object) = 0;
  virtual void visit(GeometryObject& object) = 0;

  virtual void visit(const LightObject& object) = 0;
  virtual void visit(LightObject& object) = 0;

  virtual void visit(const LODObject& object) = 0;
  virtual void visit(LODObject& object) = 0;

  virtual void visit(const MaterialObject& object) = 0;
  virtual void visit(MaterialObject& object) = 0;

  virtual void visit(const MeshObject& object) = 0;
  virtual void visit(MeshObject& object) = 0;

  virtual void visit(const MovableObject& object) = 0;
  virtual void visit(MovableObject& object) = 0;

  virtual void visit(const Object3D& object) = 0;
  virtual void visit(Object3D& object) = 0;

  virtual void visit(const RootObject& object) = 0;
  virtual void visit(RootObject& object) = 0;

  virtual void visit(const RotatableObject& object) = 0;
  virtual void visit(RotatableObject& object) = 0;

  virtual void visit(const ScalableObject& object) = 0;
  virtual void visit(ScalableObject& object) = 0;

  virtual void visit(const SkeletonGroup& object) = 0;
  virtual void visit(SkeletonGroup& object) = 0;

  virtual void visit(const SkeletonObject& object) = 0;
  virtual void visit(SkeletonObject& object) = 0;
};