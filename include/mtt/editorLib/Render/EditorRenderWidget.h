#pragma once

#include <mtt/application/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/Widgets/RenderWidget/ClickSelectHandler.h>
#include <mtt/clPipeline/ColorFrameBuilder.h>
#include <mtt/editorLib/Manipulator/EditorManipulatorController.h>
#include <mtt/render/SceneGraph/CameraNode.h>

namespace mtt
{
  class RenderScene;
  class CommonEditData;

  class EditorRenderWidget : public RenderWidget
  {
    Q_OBJECT

  public:
    explicit EditorRenderWidget(RenderScene& renderScene,
                                CommonEditData& commonEditData);
    EditorRenderWidget(const RenderWidget&) = delete;
    EditorRenderWidget& operator = (const EditorRenderWidget&) = delete;
    virtual ~EditorRenderWidget() noexcept = default;

    inline OrbitalCameraController& cameraController() noexcept;
    inline const OrbitalCameraController& cameraController() const noexcept;

    inline EditorManipulatorController& manipulatorController() noexcept;
    inline const EditorManipulatorController&
                                        manipulatorController() const noexcept;

  protected:
    virtual std::unique_ptr<RenderSceneAction> buildRenderAction(
                                    RenderScene& scene,
                                    CameraNode& camera,
                                    SceneToSurfaceRenderer& renderer) override;

  private:
    clPipeline::ColorFrameBuilder _colorFrameBuilder;

    ClickSelectHandler::FrameBuiled _uidFrameBuilder;
    SceneToImageRenderer _uidRenderer;

    OrbitalCameraController::FrameBuiled _depthMapFrameBuilder;
    SceneToImageRenderer _depthRenderer;

    CameraNode _camera;
    OrbitalCameraController _cameraController;
    EditorManipulatorController _manipulatorController;
    ClickSelectHandler _clickSelectHandler;
  };

  inline OrbitalCameraController&
                                EditorRenderWidget::cameraController() noexcept
  {
    return _cameraController;
  }

  inline const OrbitalCameraController&
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
}