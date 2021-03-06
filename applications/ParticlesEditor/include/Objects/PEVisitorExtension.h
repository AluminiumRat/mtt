#pragma once

#include <mtt/application/Scene/ObjectVisitor.h>

class AnimationAction;
class BlockerObject;
class ParticlesEmissionAction;
class EmitterObject;
class FluidModificator;
class FluidObject;
class FrameObject;
class GasEmissionAction;
class GasSource;
class GravityModificator;
class HeatingAction;
class HeaterObject;
class HierarhicalObject;
class ModificatorGroup;
class ModificatorObject;
class ParticleAnimation;
class ParticleField;
class RootObject;
class SizeControlObject;
class VisibilityControlObject;

class PEVisitorExtension
{
public:
  static mtt::ObjectVisitor::ExtensionID extensionID;

public:
  virtual void visitConstAnimationAction(const AnimationAction& object) = 0;
  virtual void visitAnimationAction(AnimationAction& object) = 0;

  virtual void visitConstBlockerObject(const BlockerObject& object) = 0;
  virtual void visitBlockerObject(BlockerObject& object) = 0;

  virtual void visitConstEmitterObject(const EmitterObject& object) = 0;
  virtual void visitEmitterObject(EmitterObject& object) = 0;

  virtual void visitConstFluidModificator(const FluidModificator& object) = 0;
  virtual void visitFluidModificator(FluidModificator& object) = 0;

  virtual void visitConstFluidObject(const FluidObject& object) = 0;
  virtual void visitFluidObject(FluidObject& object) = 0;

  virtual void visitConstFrameObject(const FrameObject& object) = 0;
  virtual void visitFrameObject(FrameObject& object) = 0;

  virtual void visitConstGasEmissionAction(const GasEmissionAction& object) = 0;
  virtual void visitGasEmissionAction(GasEmissionAction& object) = 0;

  virtual void visitConstGasSource(const GasSource& object) = 0;
  virtual void visitGasSource(GasSource& object) = 0;

  virtual void visitConstGravityModificator(
                                          const GravityModificator& object) = 0;
  virtual void visitGravityModificator(GravityModificator& object) = 0;

  virtual void visitConstHeatingAction(const HeatingAction& object) = 0;
  virtual void visitHeatingAction(HeatingAction& object) = 0;

  virtual void visitConstHeaterObject(const HeaterObject& object) = 0;
  virtual void visitHeaterObject(HeaterObject& object) = 0;

  virtual void visitConstHierarhicalObject(const HierarhicalObject& object) = 0;
  virtual void visitHierarhicalObject(HierarhicalObject& object) = 0;

  virtual void visitConstModificatorGroup(const ModificatorGroup& object) = 0;
  virtual void visitModificatorGroup(ModificatorGroup& object) = 0;

  virtual void visitConstModificatorObject(const ModificatorObject& object) = 0;
  virtual void visitModificatorObject(ModificatorObject& object) = 0;

  virtual void visitConstParticleAnimation(const ParticleAnimation& object) = 0;
  virtual void visitParticleAnimation(ParticleAnimation& object) = 0;

  virtual void visitConstParticleField(const ParticleField& object) = 0;
  virtual void visitParticleField(ParticleField& object) = 0;

  virtual void visitConstParticlesEmissionAction(
                                    const ParticlesEmissionAction& object) = 0;
  virtual void visitParticlesEmissionAction(
                                          ParticlesEmissionAction& object) = 0;

  virtual void visitConstRootObject(const RootObject& object) = 0;
  virtual void visitRootObject(RootObject& object) = 0;

  virtual void visitConstSizeControlObject(
                                          const SizeControlObject& object) = 0;
  virtual void visitSizeControlObject(SizeControlObject& object) = 0;

  virtual void visitConstVisibilityControlObject(
                                    const VisibilityControlObject& object) = 0;
  virtual void visitVisibilityControlObject(
                                          VisibilityControlObject& object) = 0;
};