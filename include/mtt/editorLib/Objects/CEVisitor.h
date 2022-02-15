#pragma once

#include <mtt/application/Scene/ObjectVisitor.h>
#include <mtt/editorLib/Objects/CEVisitorExtension.h>

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

    inline virtual void visit(const AmbientLightObject& object) override;
    inline virtual void visit(AmbientLightObject& object) override;

    inline virtual void visit(const AnimationGroup& object) override;
    inline virtual void visit(AnimationGroup& object) override;

    inline virtual void visit(const AnimationObject& object) override;
    inline virtual void visit(AnimationObject& object) override;

    inline virtual void visit(const AnimationTrack& object) override;
    inline virtual void visit(AnimationTrack& object) override;

    inline virtual void visit(const BackgroundObject& object) override;
    inline virtual void visit(BackgroundObject& object) override;

    inline virtual void visit(const CubemapObject& object) override;
    inline virtual void visit(CubemapObject& object) override;

    inline virtual void visit(const DirectLightObject& object) override;
    inline virtual void visit(DirectLightObject& object) override;

    inline virtual void visit(const DisplayedObject& object) override;
    inline virtual void visit(DisplayedObject& object) override;

    inline virtual void visit(const EnvironmentGroup& object) override;
    inline virtual void visit(EnvironmentGroup& object) override;

    inline virtual void visit(const EnvironmentModel& object) override;
    inline virtual void visit(EnvironmentModel& object) override;

    inline virtual void visit(const EnvironmentObject& object) override;
    inline virtual void visit(EnvironmentObject& object) override;

    inline virtual void visit(const LightObject& object) override;
    inline virtual void visit(LightObject& object) override;

    inline virtual void visit(const MovableObject& object) override;
    inline virtual void visit(MovableObject& object) override;

    inline virtual void visit(const Object3D& object) override;
    inline virtual void visit(Object3D& object) override;

    inline virtual void visit(const RotatableObject& object) override;
    inline virtual void visit(RotatableObject& object) override;

    inline virtual void visit(const ScalableObject& object) override;
    inline virtual void visit(ScalableObject& object) override;

    inline virtual void visit(const SkeletonGroup& object) override;
    inline virtual void visit(SkeletonGroup& object) override;

    inline virtual void visit(const SkeletonObject& object) override;
    inline virtual void visit(SkeletonObject& object) override;

    template<typename ObjectClass>
    inline void visit(ObjectClass& objectRef);
    template<typename ObjectClass>
    inline void visit(const ObjectClass& objectRef);

  protected:
    inline virtual void* getExtension(
                              ObjectVisitor::ExtensionID id) noexcept override;
  };

  using CEVisitor = CEVisitorT<ObjectVisitor>;
}

#include <mtt/editorLib/Objects/AmbientLightObject.h>
#include <mtt/editorLib/Objects/AnimationGroup.h>
#include <mtt/editorLib/Objects/AnimationObject.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Objects/CubemapObject.h>
#include <mtt/editorLib/Objects/DirectLightObject.h>
#include <mtt/editorLib/Objects/DisplayedObject.h>
#include <mtt/editorLib/Objects/EnvironmentGroup.h>
#include <mtt/editorLib/Objects/EnvironmentModel.h>
#include <mtt/editorLib/Objects/EnvironmentObject.h>
#include <mtt/editorLib/Objects/LightObject.h>
#include <mtt/editorLib/Objects/MovableObject.h>
#include <mtt/editorLib/Objects/Object3D.h>
#include <mtt/editorLib/Objects/RotatableObject.h>
#include <mtt/editorLib/Objects/ScalableObject.h>
#include <mtt/editorLib/Objects/SkeletonGroup.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>

namespace mtt
{
  template <typename BaseVisitor>
  template <typename... Args>
  inline CEVisitorT<BaseVisitor>::CEVisitorT(Args&&... args) :
    BaseVisitor(args...)
  {
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const AnimationGroup& object)
  {
    BaseVisitor* baseVisitor = this;
    baseVisitor->visit(static_cast<const Object&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(AnimationGroup& object)
  {
    BaseVisitor* baseVisitor = this;
    baseVisitor->visit(static_cast<Object&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const AnimationObject& object)
  {
    BaseVisitor* baseVisitor = this;
    baseVisitor->visit(static_cast<const Object&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(AnimationObject& object)
  {
    BaseVisitor* baseVisitor = this;
    baseVisitor->visit(static_cast<Object&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const AnimationTrack& object)
  {
    BaseVisitor* baseVisitor = this;
    baseVisitor->visit(static_cast<const Object&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(AnimationTrack& object)
  {
    BaseVisitor* baseVisitor = this;
    baseVisitor->visit(static_cast<Object&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(AmbientLightObject& object)
  {
    visit(static_cast<LightObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const AmbientLightObject& object)
  {
    visit(static_cast<const LightObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const BackgroundObject& object)
  {
    visit(static_cast<const RotatableObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(BackgroundObject& object)
  {
    visit(static_cast<RotatableObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const CubemapObject& object)
  {
    BaseVisitor* baseVisitor = this;
    baseVisitor->visit(static_cast<const Object&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(CubemapObject& object)
  {
    BaseVisitor* baseVisitor = this;
    baseVisitor->visit(static_cast<Object&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(DirectLightObject& object)
  {
    visit(static_cast<LightObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const DirectLightObject& object)
  {
    visit(static_cast<const LightObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const DisplayedObject& object)
  {
    BaseVisitor* baseVisitor = this;
    baseVisitor->visit(static_cast<const Object&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(DisplayedObject& object)
  {
    BaseVisitor* baseVisitor = this;
    baseVisitor->visit(static_cast<Object&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const EnvironmentGroup& object)
  {
    visit(static_cast<const DisplayedObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(EnvironmentGroup& object)
  {
    visit(static_cast<DisplayedObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const EnvironmentModel& object)
  {
    visit(static_cast<const EnvironmentObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(EnvironmentModel& object)
  {
    visit(static_cast<EnvironmentObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const EnvironmentObject& object)
  {
    visit(static_cast<const MovableObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(EnvironmentObject& object)
  {
    visit(static_cast<MovableObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(LightObject& object)
  {
    visit(static_cast<EnvironmentObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const LightObject& object)
  {
    visit(static_cast<const EnvironmentObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const MovableObject& object)
  {
    visit(static_cast<const RotatableObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(MovableObject& object)
  {
    visit(static_cast<RotatableObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const Object3D& object)
  {
    visit(static_cast<const DisplayedObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(Object3D& object)
  {
    visit(static_cast<DisplayedObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const RotatableObject& object)
  {
    visit(static_cast<const Object3D&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(RotatableObject& object)
  {
    visit(static_cast<Object3D&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const ScalableObject& object)
  {
    visit(static_cast<const MovableObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(ScalableObject& object)
  {
    visit(static_cast<MovableObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const SkeletonGroup& object)
  {
    visit(static_cast<const DisplayedObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(SkeletonGroup& object)
  {
    visit(static_cast<DisplayedObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(const SkeletonObject& object)
  {
    visit(static_cast<const ScalableObject&>(object));
  }

  template <typename BaseVisitor>
  inline void CEVisitorT<BaseVisitor>::visit(SkeletonObject& object)
  {
    visit(static_cast<ScalableObject&>(object));
  }

  template <typename BaseVisitor>
  template<typename ObjectClass>
  inline void CEVisitorT<BaseVisitor>::visit(ObjectClass& objectRef)
  {
    BaseVisitor::visit(objectRef);
  }

  template <typename BaseVisitor>
  template<typename ObjectClass>
  inline void CEVisitorT<BaseVisitor>::visit(const ObjectClass& objectRef)
  {
    BaseVisitor::visit(objectRef);
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