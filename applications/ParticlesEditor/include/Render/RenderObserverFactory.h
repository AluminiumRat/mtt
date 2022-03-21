#pragma once

#include <mtt/editorLib/Render/RenderObserverFactory.h>

#include <Objects/PEVisitor.h>

class RenderObserverFactory : public PEVisitorT<mtt::RenderObserverFactory>
{
public:
  RenderObserverFactory(mtt::CommonEditData& commonData);

  virtual void visitEmitterObject(EmitterObject& object) override;
  virtual void visitFrameObject(FrameObject& object) override;
  virtual void visitParticleField(ParticleField& object) override;
  virtual void visitVisibilityControlObject(VisibilityControlObject& object);
};