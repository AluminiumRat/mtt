#pragma once

#include <mtt/application/Scene/ObjectVisitor.h>

#include <Objects/OEVisitorExtension.h>

template <typename BaseVisitor>
class OEVisitorT :  public BaseVisitor,
                    public OEVisitorExtension
{
public:
  template <typename... Args>
  inline OEVisitorT(Args&&... args);
  OEVisitorT(const OEVisitorT&) = delete;
  OEVisitorT& operator = (const OEVisitorT&) = delete;
  virtual ~OEVisitorT() noexcept = default;

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

  inline virtual void visit(const BoneRefBatch& object) override;
  inline virtual void visit(BoneRefBatch& object) override;

  inline virtual void visit(const BoneRefObject& object) override;
  inline virtual void visit(BoneRefObject& object) override;

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

  inline virtual void visit(const GeometryGroup& object) override;
  inline virtual void visit(GeometryGroup& object) override;

  inline virtual void visit(const GeometryObject& object) override;
  inline virtual void visit(GeometryObject& object) override;

  inline virtual void visit(const LightObject& object) override;
  inline virtual void visit(LightObject& object) override;

  inline virtual void visit(const LODObject& object) override;
  inline virtual void visit(LODObject& object) override;

  inline virtual void visit(const MaterialObject& object) override;
  inline virtual void visit(MaterialObject& object) override;

  inline virtual void visit(const MaterialsGroup& object) override;
  inline virtual void visit(MaterialsGroup& object) override;

  inline virtual void visit(const MeshObject& object) override;
  inline virtual void visit(MeshObject& object) override;

  inline virtual void visit(const MovableObject& object) override;
  inline virtual void visit(MovableObject& object) override;

  inline virtual void visit(const Object3D& object) override;
  inline virtual void visit(Object3D& object) override;

  inline virtual void visit(const RootObject& object) override;
  inline virtual void visit(RootObject& object) override;

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
                          mtt::ObjectVisitor::ExtensionID id) noexcept override;
};

using OEVisitor = OEVisitorT<mtt::ObjectVisitor>;

#include <Objects/AmbientLightObject.h>
#include <Objects/AnimationGroup.h>
#include <Objects/AnimationObject.h>
#include <Objects/AnimationTrack.h>
#include <Objects/BackgroundObject.h>
#include <Objects/BoneRefBatch.h>
#include <Objects/BoneRefObject.h>
#include <Objects/CubemapObject.h>
#include <Objects/DirectLightObject.h>
#include <Objects/DisplayedObject.h>
#include <Objects/EnvironmentGroup.h>
#include <Objects/EnvironmentModel.h>
#include <Objects/EnvironmentObject.h>
#include <Objects/GeometryGroup.h>
#include <Objects/GeometryObject.h>
#include <Objects/LightObject.h>
#include <Objects/LODObject.h>
#include <Objects/MaterialObject.h>
#include <Objects/MaterialsGroup.h>
#include <Objects/MeshObject.h>
#include <Objects/MovableObject.h>
#include <Objects/Object3D.h>
#include <Objects/RootObject.h>
#include <Objects/RotatableObject.h>
#include <Objects/ScalableObject.h>
#include <Objects/SkeletonGroup.h>
#include <Objects/SkeletonObject.h>

template <typename BaseVisitor>
template <typename... Args>
inline OEVisitorT<BaseVisitor>::OEVisitorT(Args&&... args) :
  BaseVisitor(args...)
{
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(AmbientLightObject& object)
{
  visit(static_cast<LightObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const AmbientLightObject& object)
{
  visit(static_cast<const LightObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const AnimationGroup& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(AnimationGroup& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const AnimationObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(AnimationObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const AnimationTrack& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(AnimationTrack& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const BackgroundObject& object)
{
  visit(static_cast<const RotatableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(BackgroundObject& object)
{
  visit(static_cast<RotatableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const BoneRefBatch& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(BoneRefBatch& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const BoneRefObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(BoneRefObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const CubemapObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(CubemapObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(DirectLightObject& object)
{
  visit(static_cast<LightObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const DirectLightObject& object)
{
  visit(static_cast<const LightObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const DisplayedObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(DisplayedObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const EnvironmentGroup& object)
{
  visit(static_cast<const DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(EnvironmentGroup& object)
{
  visit(static_cast<DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const EnvironmentModel& object)
{
  visit(static_cast<const EnvironmentObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(EnvironmentModel& object)
{
  visit(static_cast<EnvironmentObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const EnvironmentObject& object)
{
  visit(static_cast<const MovableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(EnvironmentObject& object)
{
  visit(static_cast<MovableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const GeometryGroup& object)
{
  visit(static_cast<const DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(GeometryGroup& object)
{
  visit(static_cast<DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const GeometryObject& object)
{
  visit(static_cast<const ScalableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(GeometryObject& object)
{
  visit(static_cast<ScalableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(LightObject& object)
{
  visit(static_cast<EnvironmentObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const LightObject& object)
{
  visit(static_cast<const EnvironmentObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const LODObject& object)
{
  visit(static_cast<const DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(LODObject& object)
{
  visit(static_cast<DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const MaterialObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(MaterialObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(MaterialsGroup& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const MaterialsGroup& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const MeshObject& object)
{
  visit(static_cast<const GeometryObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(MeshObject& object)
{
  visit(static_cast<GeometryObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const MovableObject& object)
{
  visit(static_cast<const RotatableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(MovableObject& object)
{
  visit(static_cast<RotatableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const Object3D& object)
{
  visit(static_cast<const DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(Object3D& object)
{
  visit(static_cast<DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const RootObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(RootObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::Object&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const RotatableObject& object)
{
  visit(static_cast<const Object3D&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(RotatableObject& object)
{
  visit(static_cast<Object3D&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const ScalableObject& object)
{
  visit(static_cast<const MovableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(ScalableObject& object)
{
  visit(static_cast<MovableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const SkeletonGroup& object)
{
  visit(static_cast<const DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(SkeletonGroup& object)
{
  visit(static_cast<DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const SkeletonObject& object)
{
  visit(static_cast<const ScalableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(SkeletonObject& object)
{
  visit(static_cast<ScalableObject&>(object));
}

template <typename BaseVisitor>
template<typename ObjectClass>
inline void OEVisitorT<BaseVisitor>::visit(ObjectClass& objectRef)
{
  BaseVisitor::visit(objectRef);
}

template <typename BaseVisitor>
template<typename ObjectClass>
inline void OEVisitorT<BaseVisitor>::visit(const ObjectClass& objectRef)
{
  BaseVisitor::visit(objectRef);
}

template <typename BaseVisitor>
inline void* OEVisitorT<BaseVisitor>::getExtension(
                                    mtt::ObjectVisitor::ExtensionID id) noexcept
{
  if(id == OEVisitorExtension::extensionID)
  {
    OEVisitorExtension* extension = this;
    return extension;
  }
  else return BaseVisitor::getExtension(id);
}
