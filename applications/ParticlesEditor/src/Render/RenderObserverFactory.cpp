#include <mtt/editorLib/Render/CrossRenderObserver.h>

#include <Render/BlockerRenderObserver.h>
#include <Render/EmitterRenderObserver.h>
#include <Render/FieldRenderObserver.h>
#include <Render/HeaterRenderObserver.h>
#include <Render/ObserverWithIcon.h>
#include <Render/RenderObserverFactory.h>

#define ICON_SIZE 32

RenderObserverFactory::RenderObserverFactory(mtt::CommonEditData& commonData) :
  PEVisitorT(commonData)
{
}

void RenderObserverFactory::visitBlockerObject(BlockerObject& object)
{
  setResult(std::make_unique<BlockerRenderObserver>(object, commonData()));
}

void RenderObserverFactory::visitEmitterObject(EmitterObject& object)
{
  setResult(std::make_unique<EmitterRenderObserver>(object, commonData()));
}

void RenderObserverFactory::visitFrameObject(FrameObject& object)
{
  setResult(std::make_unique<mtt::CrossRenderObserver>(object, commonData()));
}

void RenderObserverFactory::visitGravityModificator(GravityModificator& object)
{
  setResult(std::make_unique<ObserverWithIcon>(
                                      object,
                                      commonData(),
                                      ":/particlesEditor/gravity.png",
                                      ICON_SIZE));
}

void RenderObserverFactory::visitHeaterObject(HeaterObject& object)
{
  setResult(std::make_unique<HeaterRenderObserver>(object, commonData()));
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
