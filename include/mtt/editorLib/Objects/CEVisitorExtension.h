#pragma once

#include <mtt/application/Scene/ObjectVisitor.h>

namespace mtt
{
  class AmbientLightObject;
  class AnimationGroup;
  class AnimationObject;
  class AnimationTrack;
  class BackgroundObject;
  class CubemapObject;
  class DirectLightObject;
  class DisplayedObject;
  class EnvironmentGroup;
  class EnvironmentModel;
  class EnvironmentObject;
  class LightObject;
  class MovableObject;
  class Object3D;
  class RotatableObject;
  class ScalableObject;
  class SkeletonGroup;
  class SkeletonObject;

  class CEVisitorExtension
  {
  public:
    static ObjectVisitor::ExtensionID extensionID;

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

    virtual void visit(const CubemapObject& object) = 0;
    virtual void visit(CubemapObject& object) = 0;

    virtual void visit(const DirectLightObject& object) = 0;
    virtual void visit(DirectLightObject& object) = 0;

    virtual void visit(const DisplayedObject& object) = 0;
    virtual void visit(DisplayedObject& object) = 0;

    virtual void visit(const EnvironmentGroup& object) = 0;
    virtual void visit(EnvironmentGroup& object) = 0;

    virtual void visit(const EnvironmentModel& object) = 0;
    virtual void visit(EnvironmentModel& object) = 0;

    virtual void visit(const EnvironmentObject& object) = 0;
    virtual void visit(EnvironmentObject& object) = 0;

    virtual void visit(const LightObject& object) = 0;
    virtual void visit(LightObject& object) = 0;

    virtual void visit(const MovableObject& object) = 0;
    virtual void visit(MovableObject& object) = 0;

    virtual void visit(const Object3D& object) = 0;
    virtual void visit(Object3D& object) = 0;

    virtual void visit(const RotatableObject& object) = 0;
    virtual void visit(RotatableObject& object) = 0;

    virtual void visit(const ScalableObject& object) = 0;
    virtual void visit(ScalableObject& object) = 0;

    virtual void visit(const SkeletonGroup& object) = 0;
    virtual void visit(SkeletonGroup& object) = 0;

    virtual void visit(const SkeletonObject& object) = 0;
    virtual void visit(SkeletonObject& object) = 0;
  };
}