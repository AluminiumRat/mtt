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

  inline virtual void visitConstBoneRefBatch(
                                          const BoneRefBatch& object) override;
  inline virtual void visitBoneRefBatch(BoneRefBatch& object) override;

  inline virtual void visitConstBoneRefObject(
                                          const BoneRefObject& object) override;
  inline virtual void visitBoneRefObject(BoneRefObject& object) override;

  inline virtual void visitConstGeometryGroup(
                                          const GeometryGroup& object) override;
  inline virtual void visitGeometryGroup(GeometryGroup& object) override;

  inline virtual void visitConstGeometryObject(
                                        const GeometryObject& object) override;
  inline virtual void visitGeometryObject(GeometryObject& object) override;

  inline virtual void visitConstLODObject(
                                              const LODObject& object) override;
  inline virtual void visitLODObject(LODObject& object) override;

  inline virtual void visitConstMaterialObject(
                                        const MaterialObject& object) override;
  inline virtual void visitMaterialObject(MaterialObject& object) override;

  inline virtual void visitConstMaterialsGroup(
                                        const MaterialsGroup& object) override;
  inline virtual void visitMaterialsGroup(MaterialsGroup& object) override;

  inline virtual void visitConstMeshObject(const MeshObject& object) override;
  inline virtual void visitMeshObject(MeshObject& object) override;

  inline virtual void visitConstRootObject(const RootObject& object) override;
  inline virtual void visitRootObject(RootObject& object) override;

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
inline void OEVisitorT<BaseVisitor>::visitConstBoneRefBatch(
                                                    const BoneRefBatch& object)
{
  static_cast<BaseVisitor*>(this)->visitConstObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitBoneRefBatch(BoneRefBatch& object)
{
  static_cast<BaseVisitor*>(this)->visitObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitConstBoneRefObject(
                                                    const BoneRefObject& object)
{
  static_cast<BaseVisitor*>(this)->visitConstObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitBoneRefObject(BoneRefObject& object)
{
  static_cast<BaseVisitor*>(this)->visitObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitConstGeometryGroup(
                                                    const GeometryGroup& object)
{
  static_cast<BaseVisitor*>(this)->visitConstDisplayedObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitGeometryGroup(GeometryGroup& object)
{
  static_cast<BaseVisitor*>(this)->visitDisplayedObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitConstGeometryObject(
                                                  const GeometryObject& object)
{
  static_cast<BaseVisitor*>(this)->visitConstScalableObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitGeometryObject(GeometryObject& object)
{
  static_cast<BaseVisitor*>(this)->visitScalableObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitConstLODObject(
                                                      const LODObject& object)
{
  static_cast<BaseVisitor*>(this)->visitConstDisplayedObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitLODObject(LODObject& object)
{
  static_cast<BaseVisitor*>(this)->visitDisplayedObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitConstMaterialObject(
                                                  const MaterialObject& object)
{
  static_cast<BaseVisitor*>(this)->visitConstObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitMaterialObject(MaterialObject& object)
{
  static_cast<BaseVisitor*>(this)->visitObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitMaterialsGroup(MaterialsGroup& object)
{
  static_cast<BaseVisitor*>(this)->visitObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitConstMaterialsGroup(
                                                  const MaterialsGroup& object)
{
  static_cast<BaseVisitor*>(this)->visitConstObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitConstMeshObject(
                                                      const MeshObject& object)
{
  visitConstGeometryObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitMeshObject(MeshObject& object)
{
  visitGeometryObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitConstRootObject(
                                                      const RootObject& object)
{
  static_cast<BaseVisitor*>(this)->visitConstObject(object);
}

template <typename BaseVisitor>
inline void OEVisitorT<BaseVisitor>::visitRootObject(RootObject& object)
{
  static_cast<BaseVisitor*>(this)->visitObject(object);
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
