#pragma once

#include <mtt/application/Scene/ObjectVisitor.h>
#include <mtt/editorLib/Objects/AmbientLightObject.h>
#include <mtt/editorLib/Objects/AnimationGroup.h>
#include <mtt/editorLib/Objects/AnimationObject.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Objects/CEVisitorExtension.h>
#include <mtt/editorLib/Objects/CubemapObject.h>
#include <mtt/editorLib/Objects/DirectLightObject.h>
#include <mtt/editorLib/Objects/DisplayedObject.h>
#include <mtt/editorLib/Objects/EnvironmentGroup.h>
#include <mtt/editorLib/Objects/EnvironmentModel.h>
#include <mtt/editorLib/Objects/EnvironmentObject.h>
#include <mtt/editorLib/Objects/EnvironmentRootObject.h>
#include <mtt/editorLib/Objects/LightObject.h>
#include <mtt/editorLib/Objects/MovableObject.h>
#include <mtt/editorLib/Objects/Object3D.h>
#include <mtt/editorLib/Objects/positionAnimator.h>
#include <mtt/editorLib/Objects/RotatableObject.h>
#include <mtt/editorLib/Objects/ScalableObject.h>
#include <mtt/editorLib/Objects/SkeletonGroup.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>

namespace mtt
{
  template <typename BaseVisitor>
  class CEVisitorT :  public BaseVisitor,
                      public CEVisitorExtension
  {
  public:
    template <typename... Args>
    inline CEVisitorT(Args&&... args);
    CEVisitorT(const CEVisitorT&) = delete;
    CEVisitorT& operator = (const CEVisitorT&) = delete;
    virtual ~CEVisitorT() noexcept = default;

    inline virtual void visitConstAmbientLightObject(
                                    const AmbientLightObject& object) override;
    inline virtual void visitAmbientLightObject(
                                          AmbientLightObject& object) override;

    inline virtual void visitConstAnimationGroup(
                                        const AnimationGroup& object) override;
    inline virtual void visitAnimationGroup(AnimationGroup& object) override;

    inline virtual void visitConstAnimationObject(
                                        const AnimationObject& object) override;
    inline virtual void visitAnimationObject(AnimationObject& object) override;

    inline virtual void visitConstAnimationTrack(
                                        const AnimationTrack& object) override;
    inline virtual void visitAnimationTrack(AnimationTrack& object) override;

    inline virtual void visitConstBackgroundObject(
                                      const BackgroundObject& object) override;
    inline virtual void visitBackgroundObject(
                                            BackgroundObject& object) override;

    inline virtual void visitConstCubemapObject(
                                          const CubemapObject& object) override;
    inline virtual void visitCubemapObject(CubemapObject& object) override;

    inline virtual void visitConstDirectLightObject(
                                      const DirectLightObject& object) override;
    inline virtual void visitDirectLightObject(
                                            DirectLightObject& object) override;

    inline virtual void visitConstDisplayedObject(
                                        const DisplayedObject& object) override;
    inline virtual void visitDisplayedObject(DisplayedObject& object) override;

    inline virtual void visitConstEnvironmentGroup(
                                      const EnvironmentGroup& object) override;
    inline virtual void visitEnvironmentGroup(
                                            EnvironmentGroup& object) override;

    inline virtual void visitConstEnvironmentModel(
                                      const EnvironmentModel& object) override;
    inline virtual void visitEnvironmentModel(
                                            EnvironmentModel& object) override;

    inline virtual void visitConstEnvironmentObject(
                                      const EnvironmentObject& object) override;
    inline virtual void visitEnvironmentObject(
                                            EnvironmentObject& object) override;

    inline virtual void visitConstEnvironmentRootObject(
                                  const EnvironmentRootObject& object) override;
    inline virtual void visitEnvironmentRootObject(
                                        EnvironmentRootObject& object) override;

    inline virtual void visitConstLightObject(
                                            const LightObject& object) override;
    inline virtual void visitLightObject(LightObject& object) override;

    inline virtual void visitConstMovableObject(
                                          const MovableObject& object) override;
    inline virtual void visitMovableObject(MovableObject& object) override;

    inline virtual void visitConstObject3D(const Object3D& object) override;
    inline virtual void visitObject3D(Object3D& object) override;

    inline virtual void visitConstPositionAnimator(
                                      const PositionAnimator& object) override;
    inline virtual void visitPositionAnimator(
                                            PositionAnimator& object) override;

    inline virtual void visitConstRotatableObject(
                                        const RotatableObject& object) override;
    inline virtual void visitRotatableObject(RotatableObject& object) override;

    inline virtual void visitConstScalableObject(
                                        const ScalableObject& object) override;
    inline virtual void visitScalableObject(ScalableObject& object) override;

    inline virtual void visitConstSkeletonGroup(
                                          const SkeletonGroup& object) override;
    inline virtual void visitSkeletonGroup(SkeletonGroup& object) override;

    inline virtual void visitConstSkeletonObject(
                                        const SkeletonObject& object) override;
    inline virtual void visitSkeletonObject(SkeletonObject& object) override;

  protected:
    inline virtual void* getExtension(
                              ObjectVisitor::ExtensionID id) noexcept override;
  };

  using CEVisitor = CEVisitorT<ObjectVisitor>;
}

namespace mtt
{
  template <typename BaseVisitor>
  template <typename... Args>
  inline CEVisitorT<BaseVisitor>::CEVisitorT(Args&&... args) :
    BaseVisitor(args...)
  {
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstAnimationGroup(
                                                  const AnimationGroup& object)
  {
    static_cast<BaseVisitor*>(this)->visitConstObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitAnimationGroup(
                                                        AnimationGroup& object)
  {
    static_cast<BaseVisitor*>(this)->visitObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstAnimationObject(
                                                  const AnimationObject& object)
  {
    static_cast<BaseVisitor*>(this)->visitConstObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitAnimationObject(
                                                        AnimationObject& object)
  {
    static_cast<BaseVisitor*>(this)->visitObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstAnimationTrack(
                                                  const AnimationTrack& object)
  {
    static_cast<BaseVisitor*>(this)->visitConstObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitAnimationTrack(
                                                        AnimationTrack& object)
  {
    static_cast<BaseVisitor*>(this)->visitObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitAmbientLightObject(
                                                    AmbientLightObject& object)
  {
    visitLightObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstAmbientLightObject(
                                              const AmbientLightObject& object)
  {
    visitConstLightObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstBackgroundObject(
                                                const BackgroundObject& object)
  {
    visitConstRotatableObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitBackgroundObject(
                                                      BackgroundObject& object)
  {
    visitRotatableObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstCubemapObject(
                                                    const CubemapObject& object)
  {
    static_cast<BaseVisitor*>(this)->visitConstObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitCubemapObject(CubemapObject& object)
  {
    static_cast<BaseVisitor*>(this)->visitObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitDirectLightObject(
                                                      DirectLightObject& object)
  {
    visitLightObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstDirectLightObject(
                                                const DirectLightObject& object)
  {
    visitConstLightObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstDisplayedObject(
                                                  const DisplayedObject& object)
  {
    static_cast<BaseVisitor*>(this)->visitConstObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitDisplayedObject(
                                                        DisplayedObject& object)
  {
    static_cast<BaseVisitor*>(this)->visitObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstEnvironmentGroup(
                                                const EnvironmentGroup& object)
  {
    visitConstDisplayedObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitEnvironmentGroup(
                                                      EnvironmentGroup& object)
  {
    visitDisplayedObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstEnvironmentModel(
                                                const EnvironmentModel& object)
  {
    visitConstEnvironmentObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitEnvironmentModel(
                                                      EnvironmentModel& object)
  {
    visitEnvironmentObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstEnvironmentObject(
                                                const EnvironmentObject& object)
  {
    visitConstMovableObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitEnvironmentObject(
                                                      EnvironmentObject& object)
  {
    visitMovableObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstEnvironmentRootObject(
                                            const EnvironmentRootObject& object)
  {
    static_cast<BaseVisitor*>(this)->visitConstObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitEnvironmentRootObject(
                                                  EnvironmentRootObject& object)
  {
    static_cast<BaseVisitor*>(this)->visitObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitLightObject(LightObject& object)
  {
    visitEnvironmentObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstLightObject(
                                                      const LightObject& object)
  {
    visitConstEnvironmentObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstMovableObject(
                                                    const MovableObject& object)
  {
    visitConstRotatableObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitMovableObject(
                                                          MovableObject& object)
  {
    visitRotatableObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstObject3D(
                                                        const Object3D& object)
  {
    visitConstDisplayedObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitObject3D(Object3D& object)
  {
    visitDisplayedObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstPositionAnimator(
                                                const PositionAnimator& object)
  {
    visitConstAnimationTrack(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitPositionAnimator(
                                                      PositionAnimator& object)
  {
    visitAnimationTrack(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstRotatableObject(
                                                  const RotatableObject& object)
  {
    visitConstObject3D(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitRotatableObject(
                                                        RotatableObject& object)
  {
    visitObject3D(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstScalableObject
                                                  (const ScalableObject& object)
  {
    visitConstMovableObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitScalableObject(
                                                        ScalableObject& object)
  {
    visitMovableObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstSkeletonGroup(
                                                    const SkeletonGroup& object)
  {
    visitConstDisplayedObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitSkeletonGroup(SkeletonGroup& object)
  {
    visitDisplayedObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitConstSkeletonObject(
                                                  const SkeletonObject& object)
  {
    visitConstScalableObject(object);
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visitSkeletonObject(
                                                        SkeletonObject& object)
  {
    visitScalableObject(object);
  }

  template <typename BaseVisitor>
  inline void* CEVisitorT<BaseVisitor>::getExtension(
                                        ObjectVisitor::ExtensionID id) noexcept
  {
    if(id == CEVisitorExtension::extensionID)
    {
      CEVisitorExtension* extension = this;
      return extension;
    }
    else return BaseVisitor::getExtension(id);
  }
}