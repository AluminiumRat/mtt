#pragma once

namespace mtt
{
  class CameraNode;
  class RenderScene;
  struct ViewInfo;

  class AbstractSceneRenderer
  {
  public:
    AbstractSceneRenderer() = default;
    AbstractSceneRenderer(const AbstractSceneRenderer&) = delete;
    AbstractSceneRenderer& operator = (const AbstractSceneRenderer&) = delete;
    virtual ~AbstractSceneRenderer() noexcept = default;

    virtual void render(RenderScene& scene,
                        const CameraNode& camera,
                        ViewInfo* rootViewInfo) = 0;
  };
}