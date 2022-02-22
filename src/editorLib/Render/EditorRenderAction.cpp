#include <QtWidgets/QWidget>

#include <mtt/editorLib/Render/EditorRenderAction.h>
#include <mtt/render/SceneGraph/CameraNode.h>

using namespace mtt;

EditorRenderAction::EditorRenderAction( AbstractSceneRenderer& renderer,
                                        RenderScene& scene,
                                        CameraNode& camera,
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
