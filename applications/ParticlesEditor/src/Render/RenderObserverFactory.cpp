#include <mtt/editorLib/Render/CrossRenderObserver.h>

#include <Render/EmitterRenderObserver.h>
#include <Render/FieldRenderObserver.h>
#include <Render/ObserverWithIcon.h>
#include <Render/RenderObserverFactory.h>

#define ICON_SIZE 32

RenderObserverFactory::RenderObserverFactory(mtt::CommonEditData& commonData) :
  PEVisitorT(commonData)
{
}

void RenderObserverFactory::visitEmitterObject(EmitterObject& object)
{
  setResult(std::make_unique<EmitterRenderObserver>(object, commonData()));
}

void RenderObserverFactory::visitFrameObject(FrameObject& object)
{
  setResult(std::make_unique<mtt::CrossRenderObserver>(object, commonData()));
}

void RenderObserverFactory::visitParticleField(ParticleField& object)
{
  setResult(std::make_unique<FieldRenderObserver>(object, commonData()));
}

void RenderObserverFactory::visitVisibilityControlObject(
                                                VisibilityControlObject& object)
{
  setResult(std::make_unique<ObserverWithIcon>(
                                      object,
                                      commonData(),
                                      ":/particlesEditor/visibilityControl.png",
                                      ICON_SIZE));
}
