#pragma once

#include <optional>

#include <mtt/Core/WorkCycle/WorkCycle.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>

namespace mtt
{
  class CameraNode;
  class AbstractSceneRenderer;
  class RenderScene;

  class RenderSceneAction : public WorkCycle::AbstractAction
  {
  public:
    RenderSceneAction(AbstractSceneRenderer& renderer,
                      RenderScene& scene,
                      CameraNode& camera,
                      ViewInfo* rootViewInfo) noexcept;
    RenderSceneAction(const RenderSceneAction&) = delete;
    RenderSceneAction& operator = (const RenderSceneAction&) = delete;
    virtual ~RenderSceneAction() noexcept = default;

    inline AbstractSceneRenderer& renderer() const noexcept;
    inline RenderScene& scene() const noexcept;
    inline CameraNode& camera() const noexcept;

    virtual void preAsycPart() override;
    virtual void asycPart() override;
    virtual void postAsycPart() override;

  private:
    AbstractSceneRenderer& _renderer;
    RenderScene& _scene;
    CameraNode& _camera;
    std::optional<ViewInfo> _rootViewInfo;
  };

  inline AbstractSceneRenderer& RenderSceneAction::renderer() const noexcept
  {
    return _renderer;
  }

  inline RenderScene& RenderSceneAction::scene() const noexcept
  {
    return _scene;
  }

  inline CameraNode& RenderSceneAction::camera() const noexcept
  {
    return _camera;
  }
};