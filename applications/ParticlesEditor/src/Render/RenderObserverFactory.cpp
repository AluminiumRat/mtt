#include <mtt/editorLib/Render/CrossRenderObserver.h>

#include <Render/FieldRenderObserver.h>
#include <Render/RenderObserverFactory.h>

RenderObserverFactory::RenderObserverFactory(mtt::CommonEditData& commonData) :
  PEVisitorT(commonData)
{
}

void RenderObserverFactory::visitParticleField(ParticleField& object)
{
  setResult(std::make_unique<FieldRenderObserver>(object, commonData()));
}

void RenderObserverFactory::visitFrameObject(FrameObject& object)
{
  setResult(std::make_unique<mtt::CrossRenderObserver>(object, commonData()));
}
