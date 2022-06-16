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
  class EnvironmentRootObject;
  class LightObject;
  class MovableObject;
  class Object3D;
  class PointLightObject;
  class PositionAnimator;
  class RotatableObject;
  class ScalableObject;
  class SkeletonGroup;
  class SkeletonObject;
  class SpotLightObject;

  class CEVisitorExtension
  {
  public:
    static ObjectVisitor::ExtensionID extensionID;

  public:
    virtual void visitConstAmbientLightObject(
                                          const AmbientLightObject& object) = 0;
    virtual void visitAmbientLightObject(AmbientLightObject& object) = 0;

    virtual void visitConstAnimationGroup(const AnimationGroup& object) = 0;
    virtual void visitAnimationGroup(AnimationGroup& object) = 0;

    virtual void visitConstAnimationObject(const AnimationObject& object) = 0;
    virtual void visitAnimationObject(AnimationObject& object) = 0;

    virtual void visitConstAnimationTrack(const AnimationTrack& object) = 0;
    virtual void visitAnimationTrack(AnimationTrack& object) = 0;

    virtual void visitConstBackgroundObject(const BackgroundObject& object) = 0;
    virtual void visitBackgroundObject(BackgroundObject& object) = 0;

    virtual void visitConstCubemapObject(const CubemapObject& object) = 0;
    virtual void visitCubemapObject(CubemapObject& object) = 0;

    virtual void visitConstDirectLightObject(
                                          const DirectLightObject& object) = 0;
    virtual void visitDirectLightObject(DirectLightObject& object) = 0;

    virtual void visitConstDisplayedObject(const DisplayedObject& object) = 0;
    virtual void visitDisplayedObject(DisplayedObject& object) = 0;

    virtual void visitConstEnvironmentGroup(const EnvironmentGroup& object) = 0;
    virtual void visitEnvironmentGroup(EnvironmentGroup& object) = 0;

    virtual void visitConstEnvironmentModel(const EnvironmentModel& object) = 0;
    virtual void visitEnvironmentModel(EnvironmentModel& object) = 0;

    virtual void visitConstEnvironmentObject(
                                          const EnvironmentObject& object) = 0;
    virtual void visitEnvironmentObject(EnvironmentObject& object) = 0;

    virtual void visitConstEnvironmentRootObject(
                                      const EnvironmentRootObject& object) = 0;
    virtual void visitEnvironmentRootObject(EnvironmentRootObject& object) = 0;

    virtual void visitConstLightObject(const LightObject& object) = 0;
    virtual void visitLightObject(LightObject& object) = 0;

    virtual void visitConstMovableObject(const MovableObject& object) = 0;
    virtual void visitMovableObject(MovableObject& object) = 0;

    virtual void visitConstObject3D(const Object3D& object) = 0;
    virtual void visitObject3D(Object3D& object) = 0;

    virtual void visitConstPointLightObject(const PointLightObject& object) = 0;
    virtual void visitPointLightObject(PointLightObject& object) = 0;

    virtual void visitConstPositionAnimator(const PositionAnimator& object) = 0;
    virtual void visitPositionAnimator(PositionAnimator& object) = 0;

    virtual void visitConstRotatableObject(const RotatableObject& object) = 0;
    virtual void visitRotatableObject(RotatableObject& object) = 0;

    virtual void visitConstScalableObject(const ScalableObject& object) = 0;
    virtual void visitScalableObject(ScalableObject& object) = 0;

    virtual void visitConstSkeletonGroup(const SkeletonGroup& object) = 0;
    virtual void visitSkeletonGroup(SkeletonGroup& object) = 0;

    virtual void visitConstSkeletonObject(const SkeletonObject& object) = 0;
    virtual void visitSkeletonObject(SkeletonObject& object) = 0;

    virtual void visitConstSpotLightObject(const SpotLightObject& object) = 0;
    virtual void visitSpotLightObject(SpotLightObject& object) = 0;
  };
}