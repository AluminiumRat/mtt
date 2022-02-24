#pragma once

#include <mtt/editorLib/Render/RenderObserverFactory.h>

#include <Objects/PEVisitor.h>

class RenderObserverFactory : public PEVisitorT<mtt::RenderObserverFactory>
{
public:
  RenderObserverFactory(mtt::CommonEditData& commonData);

  virtual void visitParticleField(ParticleField& object) override;
};