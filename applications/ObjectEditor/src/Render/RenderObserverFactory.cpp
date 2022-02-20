#include <Render/LODRenderObserver.h>
#include <Render/RenderObserverFactory.h>

RenderObserverFactory::RenderObserverFactory(mtt::CommonEditData& commonData) :
  OEVisitorT(commonData)
{
}

void RenderObserverFactory::visitLODObject(LODObject& object)
{
  setResult(std::make_unique<LODRenderObserver>(object, commonData()));
}
