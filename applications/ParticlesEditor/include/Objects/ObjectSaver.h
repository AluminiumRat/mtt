#pragma once

#include <mtt/editorLib/Objects/ObjectSaver.h>

#include <Objects/PEVisitor.h>

class ObjectSaver : public PEVisitorT<mtt::ObjectSaver>
{
public:
  ObjectSaver() = default;
  ObjectSaver(const ObjectSaver&) = delete;
  ObjectSaver& operator = (const ObjectSaver&) = delete;
  virtual ~ObjectSaver() noexcept = default;

  virtual void visitConstAnimationAction(
                                        const AnimationAction& object) override;
  virtual void visitConstBlockerObject(const BlockerObject& object) override;
  virtual void visitConstEmitterObject(const EmitterObject& object) override;
  virtual void visitConstFrameObject(const FrameObject& object) override;
  virtual void visitConstFluidModificator(
                                      const FluidModificator& object) override;
  virtual void visitConstFluidObject(const FluidObject& object) override;
  virtual void visitConstGasEmissionAction(
                                      const GasEmissionAction& object) override;
  virtual void visitConstGasSource(const GasSource& object) override;
  virtual void visitConstGravityModificator(
                                    const GravityModificator& object) override;
  virtual void visitConstHeaterObject(const HeaterObject& object) override;
  virtual void visitConstHeatingAction(const HeatingAction& object) override;
  virtual void visitConstModificatorObject(
                                      const ModificatorObject& object) override;
  virtual void visitConstParticleAnimation(
                                      const ParticleAnimation& object) override;
  virtual void visitConstParticleField(const ParticleField& object) override;
  virtual void visitConstParticlesEmissionAction(
                                const ParticlesEmissionAction& object) override;
  virtual void visitConstSizeControlObject(
                                      const SizeControlObject& object) override;
  virtual void visitConstVisibilityControlObject(
                                const VisibilityControlObject& object) override;
};