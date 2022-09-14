#pragma once

#include <mtt/application/Scene/ObjectLoader.h>
#include <mtt/editorLib/Objects/CEVisitor.h>

namespace mtt
{
  class CEObjectLoader : public CEVisitorT<ObjectLoader>
  {
  public:
    CEObjectLoader() = default;
    CEObjectLoader(const CEObjectLoader&) = delete;
    CEObjectLoader& operator = (const CEObjectLoader&) = delete;
    virtual ~CEObjectLoader() noexcept = default;

    virtual void visitAmbientLightObject(AmbientLightObject& object) override;
    virtual void visitAnimationGroup(AnimationGroup& object) override;
    virtual void visitAnimationObject(AnimationObject& object) override;
    virtual void visitAnimationTrack(AnimationTrack& object) override;
    virtual void visitBackgroundObject(BackgroundObject& object) override;
    virtual void visitCubemapObject(CubemapObject& object) override;
    virtual void visitDirectLightObject(DirectLightObject& object) override;
    virtual void visitDisplayedObject(DisplayedObject& object) override;
    virtual void visitEnvironmentGroup(EnvironmentGroup& object) override;
    virtual void visitEnvironmentModel(EnvironmentModel& object) override;
    virtual void visitEnvironmentRootObject(
                                        EnvironmentRootObject& object) override;
    virtual void visitFadingLightObject(FadingLightObject& object) override;
    virtual void visitLightObject(LightObject& object) override;
    virtual void visitLightWithShadowsObject(
                                      LightWithShadowsObject& object) override;
    virtual void visitMovableObject(MovableObject& object) override;
    virtual void visitPointLightObject(PointLightObject& object) override;
    virtual void visitPositionAnimator(PositionAnimator& object) override;
    virtual void visitRotatableObject(RotatableObject& object) override;
    virtual void visitScalableObject(ScalableObject& object) override;
    virtual void visitSkeletonGroup(SkeletonGroup& object) override;
    virtual void visitSkeletonObject(SkeletonObject& object) override;
    virtual void visitSpotLightObject(SpotLightObject& object) override;

  protected:
    void readCubemapData(CubemapObject& object);
  };
}