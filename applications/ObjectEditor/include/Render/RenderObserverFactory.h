#pragma once

#include <mtt/editorLib/Render/RenderObserverFactory.h>

#include <Objects/OEVisitor.h>

class RenderObserverFactory : public OEVisitorT<mtt::RenderObserverFactory>
{
public:
  RenderObserverFactory(mtt::CommonEditData& commonData);

  virtual void visitLODObject(LODObject& object) override;
};