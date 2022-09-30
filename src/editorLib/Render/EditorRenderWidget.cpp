#include <mtt/application/Application.h>
#include <mtt/editorLib/Render/EditorRenderWidget.h>

using namespace mtt;

EditorRenderWidget::EditorRenderWidget( RenderScene& renderScene,
                                        CommonEditData& commonEditData) :
  _colorFrameBuilder( surfaceFormat(),
                      Application::instance().displayDevice()),
  _uidFrameBuilder( ClickSelectHandler::uidFormat,
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                    ClickSelectHandler::uidUsage,
                    Application::instance().displayDevice()),
  _uidRenderer( _uidFrameBuilder,
                Application::instance().displayDevice().drawQueue()),
  _depthMapFrameBuilder(OrbitalCameraController::depthMapFormat,
                        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                        OrbitalCameraController::depthmapImageUsage,
                        Application::instance().displayDevice()),
  _depthRenderer( _depthMapFrameBuilder,
                  Application::instance().displayDevice().drawQueue()),
  _cameraController(*this, &_depthRenderer),
  _manipulatorController(*this, commonEditData),
  _clickSelectHandler(*this, _uidRenderer, commonEditData)
{
  _camera.setPerspectiveProjection(glm::radians(45.f), 1.f, .1f, 100.f);
  setFrameBuilder(&_colorFrameBuilder);
  setSource(&renderScene, &_camera);
}
