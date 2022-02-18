#pragma once

#include <mtt/application/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/Widgets/RenderWidget/ClickSelectHandler.h>
#include <mtt/clPipeline/ColorFrameBuilder.h>
#include <mtt/editorLib/Manipulator/EditorManipulatorController.h>
#include <mtt/render/SceneGraph/CameraNode.h>

class EditorCommonData;

class EditorRenderWidget : public mtt::RenderWidget
{
  Q_OBJECT

public:
  explicit EditorRenderWidget(EditorCommonData& commonData);
  EditorRenderWidget(const RenderWidget&) = delete;
  EditorRenderWidget& operator = (const EditorRenderWidget&) = delete;
  virtual ~EditorRenderWidget() noexcept = default;

  inline mtt::OrbitalCameraController& cameraController() noexcept;
  inline const mtt::OrbitalCameraController& cameraController() const noexcept;

  inline mtt::EditorManipulatorController& manipulatorController() noexcept;
  inline const mtt::EditorManipulatorController&
                                        manipulatorController() const noexcept;

protected:
  virtual std::unique_ptr<mtt::RenderSceneAction> buildRenderAction(
                                mtt::RenderScene& scene,
                                mtt::CameraNode& camera,
                                mtt::SceneToSurfaceRenderer& renderer) override;

private:
  mtt::clPipeline::ColorFrameBuilder _colorFrameBuilder;

  mtt::ClickSelectHandler::FrameBuiled _uidFrameBuilder;
  mtt::SceneToImageRenderer _uidRenderer;

  mtt::OrbitalCameraController::FrameBuiled _depthMapFrameBuilder;
  mtt::SceneToImageRenderer _depthRenderer;

  EditorCommonData& _commonData;
  mtt::CameraNode _camera;
  mtt::OrbitalCameraController _cameraController;
  mtt::EditorManipulatorController _manipulatorController;
  mtt::ClickSelectHandler _clickSelectHandler;
};

inline mtt::OrbitalCameraController&
                                EditorRenderWidget::cameraController() noexcept
{
  return _cameraController;
}

inline const mtt::OrbitalCameraController&
                          EditorRenderWidget::cameraController() const noexcept
{
  return _cameraController;
}

inline mtt::EditorManipulatorController&
                            EditorRenderWidget::manipulatorController() noexcept
{
  return _manipulatorController;
}

inline const mtt::EditorManipulatorController&
                      EditorRenderWidget::manipulatorController() const noexcept
{
  return _manipulatorController;
}
