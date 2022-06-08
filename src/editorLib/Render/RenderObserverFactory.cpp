#include <mtt/editorLib/Render/AmbientLightRenderObserver.h>
#include <mtt/editorLib/Render/BackgroundObserver.h>
#include <mtt/editorLib/Render/CrossRenderObserver.h>
#include <mtt/editorLib/Render/DirectLightRenderObserver.h>
#include <mtt/editorLib/Render/EnvironmentModelRenderObserver.h>
#include <mtt/editorLib/Render/RenderObserverFactory.h>
#include <mtt/editorLib/Render/SpotLightRenderObserver.h>

using namespace mtt;

RenderObserverFactory::RenderObserverFactory( CommonEditData& commonData) :
  _commonData(commonData),
  _renderScene(nullptr)
{
}

std::unique_ptr<AbstractObjectRenderObserver>
                RenderObserverFactory::buildObserver( Object& object,
                                                      RenderScene& renderScene)
{
  _renderScene = &renderScene;
  process(object);
  return std::move(_result);
}

void RenderObserverFactory::visitAmbientLightObject(
                                                  AmbientLightObject& object)
{
  setResult(std::make_unique<AmbientLightRenderObserver>( object,
                                                          commonData()));
}

void RenderObserverFactory::visitBackgroundObject(BackgroundObject& object)
{
  setResult(std::make_unique<BackgroundObserver>(object, commonData()));
}

void RenderObserverFactory::visitDirectLightObject(DirectLightObject& object)
{
  if(renderScene() == nullptr) return;
  setResult(std::make_unique<DirectLightRenderObserver>(object,
                                                        commonData(),
                                                        *renderScene()));
}

void RenderObserverFactory::visitEnvironmentModel(EnvironmentModel& object)
{
  setResult(std::make_unique<EnvironmentModelRenderObserver>( object,
                                                              commonData()));
}

void RenderObserverFactory::visitSkeletonObject(SkeletonObject& object)
{
  setResult(std::make_unique<CrossRenderObserver>(object,
                                                  commonData()));
}

void RenderObserverFactory::visitSpotLightObject(SpotLightObject& object)
{
  if(renderScene() == nullptr) return;
  setResult(std::make_unique<SpotLightRenderObserver>(object,
                                                      commonData(),
                                                      *renderScene()));
}
