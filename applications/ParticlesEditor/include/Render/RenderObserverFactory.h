#pragma once

#include <mtt/editorLib/Render/RenderObserverFactory.h>

#include <Objects/PEVisitor.h>

class RenderObserverFactory : public PEVisitorT<mtt::RenderObserverFactory>
{
public:
  RenderObserverFactory(mtt::CommonEditData& commonData);

  virtual void visitBlockerObject(BlockerObject& object) override;
  virtual void visitEmitterObject(EmitterObject& object) override;
  virtual void visitFrameObject(FrameObject& object) override;
  virtual void visitGasSource(GasSource& object) override;
  virtual void visitGravityModificator(GravityModificator& object) override;
  virtual void visitHeaterObject(HeaterObject& object) override;
  virtual void visitParticleField(ParticleField& object) override;
  virtual void visitVisibilityControlObject(VisibilityControlObject& object);
};