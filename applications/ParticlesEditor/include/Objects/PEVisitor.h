#pragma once

#include <mtt/editorLib/Objects/CEVisitor.h>

#include <Objects/Fluid/BlockerObject.h>
#include <Objects/Fluid/FluidModificator.h>
#include <Objects/Fluid/FluidObject.h>
#include <Objects/Fluid/GasSource.h>
#include <Objects/Fluid/HeaterObject.h>
#include <Objects/AnimationAction.h>
#include <Objects/EmitterObject.h>
#include <Objects/FrameObject.h>
#include <Objects/GasEmissionAction.h>
#include <Objects/GravityModificator.h>
#include <Objects/HeatingAction.h>
#include <Objects/HierarhicalObject.h>
#include <Objects/ModificatorGroup.h>
#include <Objects/ModificatorObject.h>
#include <Objects/ParticleAnimation.h>
#include <Objects/ParticleField.h>
#include <Objects/ParticlesEmissionAction.h>
#include <Objects/PEVisitorExtension.h>
#include <Objects/RootObject.h>
#include <Objects/SizeControlObject.h>
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

  inline virtual void visitConstAnimationAction(
                                      const AnimationAction& object) override;
  inline virtual void visitAnimationAction(AnimationAction& object) override;

  inline virtual void visitConstBlockerObject(
                                          const BlockerObject& object) override;
  inline virtual void visitBlockerObject(BlockerObject& object) override;

  inline virtual void visitConstEmitterObject(
                                          const EmitterObject& object) override;
  inline virtual void visitEmitterObject(EmitterObject& object) override;

  inline virtual void visitConstFluidModificator(
                                      const FluidModificator& object) override;
  inline virtual void visitFluidModificator(FluidModificator& object) override;

  inline virtual void visitConstFluidObject(const FluidObject& object) override;
  inline virtual void visitFluidObject(FluidObject& object) override;

  inline virtual void visitConstFrameObject(const FrameObject& object) override;
  inline virtual void visitFrameObject(FrameObject& object) override;

  inline virtual void visitConstGasEmissionAction(
                                      const GasEmissionAction& object) override;
  inline virtual void visitGasEmissionAction(
                                            GasEmissionAction& object) override;

  inline virtual void visitConstGasSource(const GasSource& object) override;
  inline virtual void visitGasSource(GasSource& object) override;

  inline  virtual void visitConstGravityModificator(
                                    const GravityModificator& object) override;
  inline virtual void visitGravityModificator(
                                          GravityModificator& object) override;

  inline virtual void visitConstHeatingAction(
                                          const HeatingAction& object) override;
  inline virtual void visitHeatingAction(HeatingAction& object) override;

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

  inline virtual void visitConstParticlesEmissionAction(
                                const ParticlesEmissionAction& object) override;
  inline virtual void visitParticlesEmissionAction(
                                      ParticlesEmissionAction& object) override;

  inline virtual void visitConstRootObject(const RootObject& object) override;
  inline virtual void visitRootObject(RootObject& object) override;

  inline virtual void visitConstSizeControlObject(
                                      const SizeControlObject& object) override;
  inline virtual void visitSizeControlObject(
                                            SizeControlObject& object) override;

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
inline void PEVisitorT<BaseVisitor>::visitConstAnimationAction(
                                                  const AnimationAction& object)
{
  static_cast<BaseVisitor*>(this)->visitConstAnimationTrack(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitAnimationAction(
                                                        AnimationAction& object)
{
  static_cast<BaseVisitor*>(this)->visitAnimationTrack(object);
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
inline void PEVisitorT<BaseVisitor>::visitConstFluidModificator(
                                                const FluidModificator& object)
{
  visitConstModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitFluidModificator(
                                                      FluidModificator& object)
{
  visitModificatorObject(object);
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
inline void PEVisitorT<BaseVisitor>::visitConstGasEmissionAction(
                                                const GasEmissionAction& object)
{
  visitConstAnimationAction(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitGasEmissionAction(
                                                      GasEmissionAction& object)
{
  visitAnimationAction(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstGasSource(
                                                        const GasSource& object)
{
  visitConstFluidModificator(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitGasSource(GasSource& object)
{
  visitFluidModificator(object);
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
inline void PEVisitorT<BaseVisitor>::visitConstHeatingAction(
                                                  const HeatingAction& object)
{
  visitConstAnimationAction(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitHeatingAction(HeatingAction& object)
{
  visitAnimationAction(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitConstHeaterObject(
                                                    const HeaterObject& object)
{
  visitConstFluidModificator(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitHeaterObject(HeaterObject& object)
{
  visitFluidModificator(object);
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
inline void PEVisitorT<BaseVisitor>::visitConstParticlesEmissionAction(
                                          const ParticlesEmissionAction& object)
{
  visitConstAnimationAction(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitParticlesEmissionAction(
                                                ParticlesEmissionAction& object)
{
  visitAnimationAction(object);
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
inline void PEVisitorT<BaseVisitor>::visitConstSizeControlObject(
                                                const SizeControlObject& object)
{
  visitConstModificatorObject(object);
}

template <typename BaseVisitor>
inline void PEVisitorT<BaseVisitor>::visitSizeControlObject(
                                                      SizeControlObject& object)
{
  visitModificatorObject(object);
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
