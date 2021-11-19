#pragma once

#include <mtt/Core/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/Core/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/Core/Widgets/RenderWidget/ClickSelectHandler.h>
#include <mtt/DrawImpl/ColorFrameBuilder.h>
#include <mtt/Render/SceneGraph/CameraNode.h>

#include <Manipulator/EditorManipulatorController.h>

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

  inline EditorManipulatorController& manipulatorController() noexcept;
  inline const EditorManipulatorController&
                                        manipulatorController() const noexcept;

protected:
  virtual std::unique_ptr<mtt::RenderSceneAction> buildRenderAction(
                                mtt::RenderScene& scene,
                                mtt::CameraNode& camera,
                                mtt::SceneToSurfaceRenderer& renderer) override;

private:
  mtt::ColorFrameBuilder _colorFrameBuilder;

  mtt::ClickSelectHandler::FrameBuiled _uidFrameBuilder;
  mtt::SceneToImageRenderer _uidRenderer;

  mtt::OrbitalCameraController::FrameBuiled _depthMapFrameBuilder;
  mtt::SceneToImageRenderer _depthRenderer;

  EditorCommonData& _commonData;
  mtt::CameraNode _camera;
  mtt::OrbitalCameraController _cameraController;
  EditorManipulatorController _manipulatorController;
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

inline EditorManipulatorController&
                            EditorRenderWidget::manipulatorController() noexcept
{
  return _manipulatorController;
}

inline const EditorManipulatorController&
                      EditorRenderWidget::manipulatorController() const noexcept
{
  return _manipulatorController;
}
