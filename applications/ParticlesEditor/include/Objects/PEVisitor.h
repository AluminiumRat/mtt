#pragma once

#include <mtt/editorLib/Objects/CEVisitor.h>

#include <Objects/Fluid/FluidObject.h>
#include <Objects/BlockerObject.h>
#include <Objects/EmitterObject.h>
#include <Objects/FrameObject.h>
#include <Objects/GravityModificator.h>
#include <Objects/HeaterObject.h>
#include <Objects/HierarhicalObject.h>
#include <Objects/ModificatorGroup.h>
#include <Objects/ModificatorObject.h>
#include <Objects/ParticleAnimation.h>
#include <Objects/ParticleField.h>
#include <Objects/PEVisitorExtension.h>
#include <Objects/RootObject.h>
#include <Objects/VisibilityControlObject.h>

template <typename BaseVisitor>
class PEVisitorT :  public BaseVisitor,
                    public PEVisitorExtension
{
public:
  template <typename... Args>
  inline PEVisitorT(Args&&... args);
  PEVisitorT(const PEVisitorT&) = delete;
  PEVisitorT& operator = (const PEVisitorT&) = delete;
  virtual ~PEVisitorT() noexcept = default;

  inline virtual void visitConstBlockerObject(
                                          const BlockerObject& object) override;
  inline virtual void visitBlockerObject(BlockerObject& object) override;

  inline virtual void visitConstEmitterObject(
                                          const EmitterObject& object) override;
  inline virtual void visitEmitterObject(EmitterObject& object) override;

  inline virtual void visitConstFluidObject(const FluidObject& object) override;
  inline virtual void visitFluidObject(FluidObject& object) override;

  inline virtual void visitConstFrameObject(const FrameObject& object) override;
  inline virtual void visitFrameObject(FrameObject& object) override;

  inline  virtual void visitConstGravityModificator(
                                    const GravityModificator& object) override;
  inline virtual void visitGravityModificator(
                                          GravityModificator& object) override;

  inline virtual void visitConstHeaterObject(
                                          const HeaterObject& object) override;
  inline virtual void visitHeaterObject(HeaterObject& object) override;

  inline  virtual void visitConstHierarhicalObject(
                                      const HierarhicalObject& object) override;
  inline virtual void visitHierarhicalObject(
                                            HierarhicalObject& object) override;

  inline virtual void visitConstModificatorGroup(
                                      const ModificatorGroup& object) override;
  inline virtual void visitModificatorGroup(ModificatorGroup& object) override;

  inline virtual void visitConstModificatorObject(
                                      const ModificatorObject& object) override;
  inline virtual void visitModificatorObject(
                                            ModificatorObject& object) override;

  inline virtual void visitConstParticleAnimation(
                                      const ParticleAnimation& object) override;
  inline virtual void visitParticleAnimation(
                                            ParticleAnimation& object) override;

  inline virtual void visitConstParticleField(
                                          const ParticleField& object) override;
  inline virtual void visitParticleField(ParticleField& object) override;

  inline virtual void visitConstRootObject(const RootObject& object) override;
  inline virtual void visitRootObject(RootObject& object) override;

  inline virtual void visitConstVisibilityControlObject(
                                const VisibilityControlObject& object) override;
  inline virtual void visitVisibilityControlObject(
                                      VisibilityControlObject& object) override;

protected:
  inline virtual void* getExtension(
                          mtt::ObjectVisitor::ExtensionID id) noexcept override;
};

using PEVisitor = PEVisitorT<mtt::CEVisitor>;

template <typename BaseVisitor>
template <typename... Args>
inline PEVisitorT<BaseVisitor>::PEVisitorT(Args&&... args) :
  BaseVisitor(args...)
{
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstBlockerObject(
                                                    const BlockerObject& object)
{
  visitConstModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitBlockerObject(BlockerObject& object)
{
  visitModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstEmitterObject(
                                                    const EmitterObject& object)
{
  visitConstHierarhicalObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitEmitterObject(EmitterObject& object)
{
  visitHierarhicalObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstFluidObject(
                                                      const FluidObject& object)
{
  static_cast<BaseVisitor*>(this)->visitConstObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitFluidObject(FluidObject& object)
{
  static_cast<BaseVisitor*>(this)->visitObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstFrameObject(
                                                      const FrameObject& object)
{
  visitConstHierarhicalObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitFrameObject(FrameObject& object)
{
  visitHierarhicalObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstGravityModificator(
                                              const GravityModificator& object)
{
  visitConstModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitGravityModificator(
                                                    GravityModificator& object)
{
  visitModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstHeaterObject(
                                                    const HeaterObject& object)
{
  visitConstModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitHeaterObject(HeaterObject& object)
{
  visitModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstHierarhicalObject(
                                                const HierarhicalObject& object)
{
  static_cast<BaseVisitor*>(this)->visitConstScalableObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitHierarhicalObject(
                                                      HierarhicalObject& object)
{
  static_cast<BaseVisitor*>(this)->visitScalableObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstModificatorGroup(
                                                const ModificatorGroup& object)
{
  static_cast<BaseVisitor*>(this)->visitConstDisplayedObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitModificatorGroup(
                                                      ModificatorGroup& object)
{
  static_cast<BaseVisitor*>(this)->visitDisplayedObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstModificatorObject(
                                                const ModificatorObject& object)
{
  visitConstHierarhicalObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitModificatorObject(
                                                      ModificatorObject& object)
{
  visitHierarhicalObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstParticleAnimation(
                                                const ParticleAnimation& object)
{
  static_cast<BaseVisitor*>(this)->visitConstAnimationObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitParticleAnimation(
                                                      ParticleAnimation& object)
{
  static_cast<BaseVisitor*>(this)->visitAnimationObject(object);
}

template <typename BaseVisitor>
inline  void PEVisitorT<BaseVisitor>::visitConstParticleField(
                                                    const ParticleField& object)
{
  static_cast<BaseVisitor*>(this)->visitConstMovableObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitParticleField(ParticleField& object)
{
  static_cast<BaseVisitor*>(this)->visitMovableObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstRootObject(
                                                      const RootObject& object)
{
  static_cast<BaseVisitor*>(this)->visitConstObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitRootObject(RootObject& object)
{
  static_cast<BaseVisitor*>(this)->visitObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstVisibilityControlObject(
                                          const VisibilityControlObject& object)
{
  visitConstModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitVisibilityControlObject(
                                                VisibilityControlObject& object)
{
  visitModificatorObject(object);
}

template <typename BaseVisitor>
inline void* PEVisitorT<BaseVisitor>::getExtension(
                                  mtt::ObjectVisitor::ExtensionID id) noexcept
{
  if(id == PEVisitorExtension::extensionID)
  {
    PEVisitorExtension* extension = this;
    return extension;
  }
  else return BaseVisitor::getExtension(id);
}
