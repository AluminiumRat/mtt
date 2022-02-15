#pragma once

#include <mtt/editorLib/Objects/CEVisitor.h>

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

  inline virtual void visit(const BoneRefBatch& object) override;
  inline virtual void visit(BoneRefBatch& object) override;

  inline virtual void visit(const BoneRefObject& object) override;
  inline virtual void visit(BoneRefObject& object) override;

  inline virtual void visit(const GeometryGroup& object) override;
  inline virtual void visit(GeometryGroup& object) override;

  inline virtual void visit(const GeometryObject& object) override;
  inline virtual void visit(GeometryObject& object) override;

  inline virtual void visit(const LODObject& object) override;
  inline virtual void visit(LODObject& object) override;

  inline virtual void visit(const MaterialObject& object) override;
  inline virtual void visit(MaterialObject& object) override;

  inline virtual void visit(const MaterialsGroup& object) override;
  inline virtual void visit(MaterialsGroup& object) override;

  inline virtual void visit(const MeshObject& object) override;
  inline virtual void visit(MeshObject& object) override;

  inline virtual void visit(const RootObject& object) override;
  inline virtual void visit(RootObject& object) override;

  template<typename ObjectClass>
  inline void visit(ObjectClass& objectRef);
  template<typename ObjectClass>
  inline void visit(const ObjectClass& objectRef);

protected:
  inline virtual void* getExtension(
                          mtt::ObjectVisitor::ExtensionID id) noexcept override;
};

using OEVisitor = OEVisitorT<mtt::CEVisitor>;

#include <Objects/BoneRefBatch.h>
#include <Objects/BoneRefObject.h>
#include <Objects/GeometryGroup.h>
#include <Objects/GeometryObject.h>
#include <Objects/LODObject.h>
#include <Objects/MaterialObject.h>
#include <Objects/MaterialsGroup.h>
#include <Objects/MeshObject.h>
#include <Objects/RootObject.h>

template <typename BaseVisitor>
template <typename... Args>
inline OEVisitorT<BaseVisitor>::OEVisitorT(Args&&... args) :
  BaseVisitor(args...)
{
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
inline void OEVisitorT<BaseVisitor>::visit(const GeometryGroup& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(GeometryGroup& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const GeometryObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::ScalableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(GeometryObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::ScalableObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(const LODObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<const mtt::DisplayedObject&>(object));
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visit(LODObject& object)
{
  BaseVisitor* baseVisitor = this;
  baseVisitor->visit(static_cast<mtt::DisplayedObject&>(object));
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
