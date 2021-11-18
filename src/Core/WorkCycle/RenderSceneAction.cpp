#include <exception>

#include <mtt/Core/WorkCycle/RenderSceneAction.h>
#include <mtt/Render/SceneRenderer/AbstractSceneRenderer.h>
#include <mtt/Utilities/Log.h>

using namespace mtt;

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
