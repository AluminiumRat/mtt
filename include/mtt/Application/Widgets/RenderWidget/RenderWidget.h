#pragma once

#include <memory>
#include <optional>

#include <QtGui/QWindow>
#include <QtWidgets/QWidget>

#include <mtt/Application/WorkCycle/RenderSceneAction.h>
#include <mtt/Render/SceneGraph/CameraNode.h>
#include <mtt/Render/SceneRenderer/SceneToSurfaceRenderer.h>
#include <mtt/Render/RenderScene.h>
#include <mtt/Render/RenderSurface.h>

namespace mtt
{
  class RenderWidget : public QWidget
  {
    Q_OBJECT

  public:
    RenderWidget();
    RenderWidget(const RenderWidget&) = delete;
    RenderWidget& operator = (const RenderWidget&) = delete;
    virtual ~RenderWidget() noexcept;

    inline LogicalDevice& device() const noexcept;

    inline RenderScene* scene() const noexcept;
    inline CameraNode* camera() const noexcept;

    inline QWindow& renderWindow() noexcept;
    inline const QWindow& renderWindow() const noexcept;

    inline VkFormat surfaceFormat() const noexcept;

    inline AbstractColorFrameBuilder* frameBuilder() const noexcept;
    inline void setFrameBuilder(AbstractColorFrameBuilder* newBuilder);

  signals:
    void cameraChanged(CameraNode* newCamera);
    void sceneChanged(RenderScene* newScene);

  protected:
    virtual void setSource(RenderScene* scene, CameraNode* camera);
    virtual void setCamera(CameraNode* camera);

    virtual std::unique_ptr<RenderSceneAction> buildRenderAction(
                                              RenderScene& scene,
                                              CameraNode& camera,
                                              SceneToSurfaceRenderer& renderer);

    virtual void showEvent(QShowEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;

  private:
    void rebuildAction(RenderScene* scene, CameraNode* camera);
    void removeAction() noexcept;
  
  private:
    LogicalDevice& _device;

    QWindow _renderWindow;
    QWidget* _windowContainer;
    std::optional<RenderSurface> _surface;
    std::optional<SceneToSurfaceRenderer> _renderer;

    RenderScene* _scene;
    CameraNode* _camera;
    RenderSceneAction* _renderAction;
    bool _needAction;
  };

  inline LogicalDevice& RenderWidget::device() const noexcept
  {
    return _device;
  }

  inline RenderScene* RenderWidget::scene() const noexcept
  {
    return _scene;
  }

  inline CameraNode* RenderWidget::camera() const noexcept
  {
    return _camera;
  }

  inline QWindow& RenderWidget::renderWindow() noexcept
  {
    return _renderWindow;
  }

  inline const QWindow& RenderWidget::renderWindow() const noexcept
  {
    return _renderWindow;
  }

  inline VkFormat RenderWidget::surfaceFormat() const noexcept
  {
    return _renderer->targetImageFormat();
  }

  inline AbstractColorFrameBuilder* RenderWidget::frameBuilder() const noexcept
  {
    return _renderer->frameBuilder();
  }

  inline void RenderWidget::setFrameBuilder(
                                          AbstractColorFrameBuilder* newBuilder)
  {
    _renderer->setFrameBuilder(newBuilder);
  }
}