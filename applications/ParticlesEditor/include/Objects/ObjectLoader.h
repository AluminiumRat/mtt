#pragma once

#include <mtt/editorLib/Objects/CEObjectLoader.h>

#include <Objects/PEVisitor.h>

class ObjectLoader : public PEVisitorT<mtt::CEObjectLoader>
{
public:
  ObjectLoader() = default;
  ObjectLoader(const ObjectLoader&) = delete;
  ObjectLoader& operator = (const ObjectLoader&) = delete;
  virtual ~ObjectLoader() noexcept = default;

  virtual void visitAnimationAction(AnimationAction& object) override;
  virtual void visitBlockerObject(BlockerObject& object) override;
  virtual void visitEmitterObject(EmitterObject& object) override;
  virtual void visitFrameObject(FrameObject& object) override;
  virtual void visitFluidModificator(FluidModificator& object) override;
  virtual void visitFluidObject(FluidObject& object) override;
  virtual void visitGasEmissionAction(GasEmissionAction& object) override;
  virtual void visitGasSource(GasSource& object) override;
  virtual void visitGravityModificator(GravityModificator& object) override;
  virtual void visitHeaterObject(HeaterObject& object) override;
  virtual void visitHeatingAction(HeatingAction& object) override;
  virtual void visitModificatorGroup(ModificatorGroup& object) override;
  virtual void visitModificatorObject(ModificatorObject& object) override;
  virtual void visitParticleAnimation(ParticleAnimation& object) override;
  virtual void visitParticleField(ParticleField& object) override;
  virtual void visitParticlesEmissionAction(
                                      ParticlesEmissionAction& object) override;
  virtual void visitRootObject(RootObject& object) override;
  virtual void visitSizeControlObject(SizeControlObject& object) override;
  virtual void visitVisibilityControlObject(
                                      VisibilityControlObject& object) override;
};