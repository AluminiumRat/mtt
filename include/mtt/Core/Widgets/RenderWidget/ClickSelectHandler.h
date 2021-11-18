#pragma once

#include <QtCore/QObject>

#include <mtt/Render/SceneGraph/CameraNode.h>
#include <mtt/Render/SceneRenderer/SceneToImageRenderer.h>
#include <mtt/Render/SceneRenderer/UidFrameBuilder.h>

class QMouseEvent;

namespace mtt
{
  class CommonEditData;
  class RenderScene;
  class RenderWidget;

  class ClickSelectHandler : public QObject
  {
    Q_OBJECT

  public:
    /// R component is low part of uid, G component is high part;
    static constexpr VkFormat uidFormat = VK_FORMAT_R32G32_UINT;
    static const VkImageUsageFlags uidUsage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    using FrameBuiled = UidFrameBuilder;

  public:
    /// uidRenderer should be builded on ClickSelectHandler::FrameBuilder and
    /// ClickSelectHandler::uidFormat
    ClickSelectHandler( RenderWidget& widget,
                        SceneToImageRenderer& uidRenderer,
                        CommonEditData& commonData);
    ClickSelectHandler(const ClickSelectHandler&) = delete;
    ClickSelectHandler& operator = (const ClickSelectHandler&) = delete;
    virtual ~ClickSelectHandler() noexcept = default;
  
    virtual bool eventFilter(QObject* obj, QEvent* event) override;

  protected:
    virtual void processClickedUid( const UID& uid,
                                    Qt::KeyboardModifiers modifiers) noexcept;

  private:
    void _setCamera(CameraNode* camera) noexcept;
    void _setRenderScene(RenderScene* newScene) noexcept;
    void _processClick(QMouseEvent& mouseEvent) noexcept;

  private:
    RenderWidget& _widget;
    CameraNode* _widgetCamera;
    RenderScene* _renderScene;

    CommonEditData& _commonData;

    CameraNode _renderCamera;
    SceneToImageRenderer& _sceneRenderer;

    QPoint _clickMousePosition;
  };
}