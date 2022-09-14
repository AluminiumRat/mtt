# pragma once

#include <mtt/application/Scene/ObjectSaver.h>
#include <mtt/editorLib/Objects/CEVisitor.h>

namespace mtt
{
  class CEObjectSaver : public CEVisitorT<ObjectSaver>
  {
  public:
    CEObjectSaver() = default;
    CEObjectSaver(const CEObjectSaver&) = delete;
    CEObjectSaver& operator = (const CEObjectSaver&) = delete;
    virtual ~CEObjectSaver() noexcept = default;

    virtual void visitConstAmbientLightObject(
                                    const AmbientLightObject& object) override;
    virtual void visitConstAnimationGroup(
                                        const AnimationGroup& object) override;
    virtual void visitConstAnimationObject(
                                        const AnimationObject& object) override;
    virtual void visitConstAnimationTrack(
                                        const AnimationTrack& object) override;
    virtual void visitConstBackgroundObject(
                                      const BackgroundObject& object) override;
    virtual void visitConstCubemapObject(const CubemapObject& object) override;
    virtual void visitConstDirectLightObject(
                                      const DirectLightObject& object) override;
    virtual void visitConstDisplayedObject(
                                        const DisplayedObject& object) override;
    virtual void visitConstEnvironmentGroup(
                                      const EnvironmentGroup& object) override;
    virtual void visitConstEnvironmentModel(
                                      const EnvironmentModel& object) override;
    virtual void visitConstEnvironmentRootObject(
                                  const EnvironmentRootObject& object) override;
    virtual void visitConstFadingLightObject(
                                      const FadingLightObject& object) override;
    virtual void visitConstLightObject(const LightObject& object) override;
    virtual void visitConstLightWithShadowsObject(
                                const LightWithShadowsObject& object) override;
    virtual void visitConstMovableObject(const MovableObject& object) override;
    virtual void visitConstPointLightObject(
                                      const PointLightObject& object) override;
    virtual void visitConstPositionAnimator(
                                      const PositionAnimator& object) override;
    virtual void visitConstRotatableObject(
                                        const RotatableObject& object) override;
    virtual void visitConstScalableObject(
                                        const ScalableObject& object) override;
    virtual void visitConstSkeletonGroup(const SkeletonGroup& object) override;
    virtual void visitConstSkeletonObject(
                                        const SkeletonObject& object) override;
    virtual void visitConstSpotLightObject(
                                        const SpotLightObject& object) override;

  protected:
    void writeCubemapData(const CubemapObject& object);
  };
}