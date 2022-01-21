#include <QtWidgets/QWidget>

#include <mtt/render/SceneGraph/CameraNode.h>

#include <Render/EditorRenderAction.h>

EditorRenderAction::EditorRenderAction( mtt::AbstractSceneRenderer& renderer,
                                        mtt::RenderScene& scene,
                                        mtt::CameraNode& camera,
                                        const QWidget& target) :
  RenderSceneAction(renderer, scene, camera, nullptr),
  _target(target)
{
}

void EditorRenderAction::preAsycPart()
{
  QSize targetSize = _target.size();
  if(targetSize.height() != 0)
  {
    float aspect = float(targetSize.width()) / float(targetSize.height());
    camera().setPerspectiveProjection(glm::radians(45.f),
                                      aspect,
                                      .1f,
                                      100.f);
  }
  RenderSceneAction::preAsycPart();
}
