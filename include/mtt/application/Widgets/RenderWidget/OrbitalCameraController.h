#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <QtCore/QObject>

#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/SceneRenderer/SceneToImageRenderer.h>
#include <mtt/render/SceneRenderer/DepthFrameBuilder.h>

class QMouseEvent;

namespace mtt
{
  class RenderWidget;
  class RenderScene;

  class OrbitalCameraController : public QObject
  {
    Q_OBJECT

  public:
    static constexpr VkFormat depthMapFormat = VK_FORMAT_D32_SFLOAT;
    static constexpr VkImageUsageFlags depthmapImageUsage =
                                              VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    using FrameBuiled = DepthFrameBuilder;

  public:
    OrbitalCameraController(RenderWidget& widget,
                            SceneToImageRenderer* depthRenderer);
    OrbitalCameraController(const OrbitalCameraController&) = delete;
    OrbitalCameraController& operator = (
                                      const OrbitalCameraController&) = delete;
    virtual ~OrbitalCameraController() noexcept = default;

    inline const glm::vec3& centerPosition() const noexcept;
    inline void setCenterPosition(const glm::vec3& newValue) noexcept;

    inline float distance() const noexcept;
    inline void setDistance(float newValue) noexcept;

    inline float zAngle() const noexcept;
    inline void setZAngle(float newValue) noexcept;

    inline float yAngle() const noexcept;
    inline void setYAngle(float newValue) noexcept;

    inline float rotateSensitivity() const noexcept;
    void setRotateSensitivity(float newValue) noexcept;

    inline float movingSensitivity() const noexcept;
    void setMovingSensitivity(float newValue) noexcept;

    inline float approachSensitivity() const noexcept;
    void setApproachSensitivity(float newValue) noexcept;

    virtual bool eventFilter(QObject* obj, QEvent* event) override;
  
  private:
    void _setCamera(CameraNode* camera) noexcept;
    void _setRenderScene(RenderScene* newScene) noexcept;
    void _updateCameraPosition() noexcept;
    void _rotate(const glm::vec2& mouseMoving) noexcept;
    float _movingRate() const noexcept;
    glm::vec3 _frontDirection() const noexcept;
    glm::vec3 _rightDirection() const noexcept;
    void _moveHorizontal(const glm::vec2& mouseMoving) noexcept;
    void _moveVertical(const glm::vec2& mouseMoving) noexcept;
    void _approach(const QPoint& angleDelta) noexcept;
    void _findCenterByClick(QMouseEvent& mouseEvent) noexcept;
    void _setCenter(glm::vec3 newCenter) noexcept;
    void _updateProjectionMatrix() noexcept;

  private:
    RenderWidget& _widget;
    CameraNode* _widgetCamera;
    RenderScene* _renderScene;

    SceneToImageRenderer* _depthRenderer;
    CameraNode _depthRenderCamera;

    glm::vec3 _centerPosition;
    float _distance;
    float _zAngle;
    float _yAngle;

    Qt::MouseButton _lastPressedButton;
    QPoint _pressMousePosition;
    QPoint _lastMousePosition;
  
    float _rotateSensitivity;
    float _movingSensitivity;
    float _approachSensitivity;
  };

  inline const glm::vec3&
                        OrbitalCameraController::centerPosition() const noexcept
  {
    return _centerPosition;
  }

  inline void OrbitalCameraController::setCenterPosition(
                                            const glm::vec3& newValue) noexcept
  {
    _centerPosition = newValue;
    _updateCameraPosition();
  }

  inline float OrbitalCameraController::distance() const noexcept
  {
    return _distance;
  }

  inline void OrbitalCameraController::setDistance(float newValue) noexcept
  {
    _distance = newValue;
    _updateCameraPosition();
  }

  inline float OrbitalCameraController::zAngle() const noexcept
  {
    return _zAngle;
  }

  inline void OrbitalCameraController::setZAngle(float newValue) noexcept
  {
    _zAngle = newValue;
    _updateCameraPosition();
  }

  inline float OrbitalCameraController::yAngle() const noexcept
  {
    return _yAngle;
  }

  inline void OrbitalCameraController::setYAngle(float newValue) noexcept
  {
    _yAngle = newValue;
    _updateCameraPosition();
  }

  inline float OrbitalCameraController::rotateSensitivity() const noexcept
  {
    return _rotateSensitivity;
  }

  inline float OrbitalCameraController::movingSensitivity() const noexcept
  {
    return _movingSensitivity;
  }

  inline float OrbitalCameraController::approachSensitivity() const noexcept
  {
    return _approachSensitivity;
  }
}