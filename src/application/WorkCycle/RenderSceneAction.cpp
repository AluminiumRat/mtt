#include <exception>

#include <mtt/application/WorkCycle/RenderSceneAction.h>
#include <mtt/render/SceneRenderer/AbstractSceneRenderer.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

const int RenderSceneAction::actionCategory = WorkCycle::createActionCategory();

RenderSceneAction::RenderSceneAction( AbstractSceneRenderer& renderer,
                                      RenderScene& scene,
                                      CameraNode& camera,
                                      ViewInfo* rootViewInfo) noexcept :
  _renderer(renderer),
  _scene(scene),
  _camera(camera)
{
  if(rootViewInfo != nullptr) _rootViewInfo.emplace(*rootViewInfo);
}

void RenderSceneAction::preAsycPart()
{
}

void RenderSceneAction::asycPart()
{
  try
  {
    _renderer.render( _scene,
                      _camera,
                      _rootViewInfo.has_value() ? &_rootViewInfo.value() :
                                                  nullptr);
  }
  catch (std::exception& error)
  {
    Log() << "RenderSceneAction::asycPart: " << error.what();
  }
  catch (...)
  {
    Log() << "RenderSceneAction::asycPart: unknown error";
  }
}

void RenderSceneAction::postAsycPart()
{
}
