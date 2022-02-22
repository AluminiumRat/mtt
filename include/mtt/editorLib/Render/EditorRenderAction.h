#pragma once

#include <mtt/application/WorkCycle/RenderSceneAction.h>

class QWidget;

namespace mtt
{
  class EditorRenderAction : public RenderSceneAction
  {
  public:
    EditorRenderAction( AbstractSceneRenderer& renderer,
                        RenderScene& scene,
                        CameraNode& camera,
                        const QWidget& target);
    EditorRenderAction(const EditorRenderAction&) = delete;
    EditorRenderAction& operator = (const EditorRenderAction&) = delete;
    virtual ~EditorRenderAction() noexcept = default;

    virtual void preAsycPart() override;

  private:
    const QWidget& _target;
  };
}