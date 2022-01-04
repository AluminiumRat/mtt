#pragma once

#include <mtt/Application/WorkCycle/RenderSceneAction.h>

class QWidget;

class EditorRenderAction : public mtt::RenderSceneAction
{
public:
  EditorRenderAction( mtt::AbstractSceneRenderer& renderer,
                      mtt::RenderScene& scene,
                      mtt::CameraNode& camera,
                      const QWidget& target);
  EditorRenderAction(const EditorRenderAction&) = delete;
  EditorRenderAction& operator = (const EditorRenderAction&) = delete;
  virtual ~EditorRenderAction() noexcept = default;

  virtual void preAsycPart() override;

private:
  const QWidget& _target;
};