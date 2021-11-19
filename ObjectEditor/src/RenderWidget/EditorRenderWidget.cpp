#include <glm/gtc/matrix_transform.hpp>

#include <mtt/Core/Application.h>

#include <Render/EditorRenderAction.h>
#include <RenderWidget/EditorRenderWidget.h>
#include <EditorCommonData.h>

EditorRenderWidget::EditorRenderWidget(EditorCommonData& commonData) :
  _colorFrameBuilder( surfaceFormat(),
                      mtt::Application::instance().displayDevice()),
  _uidFrameBuilder( mtt::ClickSelectHandler::uidFormat,
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                    mtt::ClickSelectHandler::uidUsage,
                    mtt::Application::instance().displayDevice()),
  _uidRenderer( _uidFrameBuilder,
                mtt::Application::instance().displayDevice().drawQueue()),
  _depthMapFrameBuilder(mtt::OrbitalCameraController::depthMapFormat,
                        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                        mtt::OrbitalCameraController::depthmapImageUsage,
                        mtt::Application::instance().displayDevice()),
  _depthRenderer( _depthMapFrameBuilder,
                  mtt::Application::instance().displayDevice().drawQueue()),
  _commonData(commonData),
  _cameraController(*this, &_depthRenderer),
  _manipulatorController(*this, commonData),
  _clickSelectHandler(*this, _uidRenderer, commonData)
{
  setFrameBuilder(&_colorFrameBuilder);
  setSource(&_commonData.renderScene(), &_camera);
}

std::unique_ptr<mtt::RenderSceneAction>
                        EditorRenderWidget::buildRenderAction(
                                      mtt::RenderScene& scene,
                                      mtt::CameraNode& camera,
                                      mtt::SceneToSurfaceRenderer& renderer)
{
  return std::make_unique<EditorRenderAction>(renderer,
                                              scene,
                                              camera,
                                              *this);
}
