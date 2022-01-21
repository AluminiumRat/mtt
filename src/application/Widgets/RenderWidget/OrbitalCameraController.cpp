#include <optional>
#include <stdexcept>

#include <QtGui/QMouseEvent>
#include <QtGui/QInputEvent>

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include <mtt/application/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/WorkCycle/RenderImageAction.h>
#include <mtt/application/Application.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

OrbitalCameraController::OrbitalCameraController(
                                          RenderWidget& widget,
                                          SceneToImageRenderer* depthRenderer) :
  _widget(widget),
  _widgetCamera(nullptr),
  _renderScene(nullptr),
  _depthRenderer(depthRenderer),
  _centerPosition(0),
  _distance(5),
  _zAngle(-glm::pi<float>() / 4.f),
  _yAngle(-glm::pi<float>() / 4.f),
  _lastPressedButton(Qt::NoButton),
  _lastMousePosition(0,0),
  _rotateSensitivity(2.f),
  _movingSensitivity(1.f),
  _approachSensitivity(1.f / 120.f)
{
  widget.renderWindow().installEventFilter(this);

  connect(&_widget,
          &RenderWidget::cameraChanged,
          this,
          &OrbitalCameraController::_setCamera,
          Qt::DirectConnection);
  _setCamera(_widget.camera());

  connect(&_widget,
          &RenderWidget::sceneChanged,
          this,
          &OrbitalCameraController::_setRenderScene,
          Qt::DirectConnection);
  _setRenderScene(_widget.scene());
}

void OrbitalCameraController::_setCamera(CameraNode* camera) noexcept
{
  _widgetCamera = camera;
  _updateCameraPosition();
}

void OrbitalCameraController::_setRenderScene(RenderScene* newScene) noexcept
{
  _renderScene = newScene;
}

void OrbitalCameraController::_updateCameraPosition() noexcept
{
  if(_widgetCamera == nullptr) return;

  glm::mat4 viewMatrix = glm::lookAt( glm::vec3(_distance, 0.f, 0.f),
                                      glm::vec3(0, 0, 0),
                                      glm::vec3(0, 0, 1));
  glm::mat4 cameraTransform = glm::inverse(viewMatrix);

  glm::mat4 rotateTransform = glm::eulerAngleZ(_zAngle);
  rotateTransform = rotateTransform * glm::eulerAngleY(_yAngle);
  cameraTransform = rotateTransform * cameraTransform;

  glm::mat4 moveTransform = glm::translate(glm::mat4(1), _centerPosition);
  cameraTransform = moveTransform * cameraTransform;

  _widgetCamera->setTransformMatrix(cameraTransform);
}

void OrbitalCameraController::setRotateSensitivity(float newValue) noexcept
{
  _rotateSensitivity = newValue;
}

void OrbitalCameraController::setMovingSensitivity(float newValue) noexcept
{
  _movingSensitivity = newValue;
}

void OrbitalCameraController::setApproachSensitivity(float newValue) noexcept
{
  _approachSensitivity = newValue;
}

bool OrbitalCameraController::eventFilter(QObject* obj, QEvent* event)
{
  if(event->type() == QEvent::MouseButtonPress)
  {
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    _lastMousePosition = mouseEvent->pos();
    _pressMousePosition = mouseEvent->pos();
    _lastPressedButton = mouseEvent->button();
  }

  if(event->type() == QEvent::MouseButtonRelease)
  {
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    
    if(_lastPressedButton == mouseEvent->button())
    {
      if( _lastPressedButton == Qt::MiddleButton &&
          _pressMousePosition == mouseEvent->pos())
      {
        _findCenterByClick(*mouseEvent);
      }

      _lastPressedButton = Qt::NoButton;
    }
  }

  if( event->type() == QEvent::MouseMove  &&
      _lastPressedButton != Qt::NoButton &&
      _widget.width() != 0 &&
      _widget.height() != 0)
  {
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    QPoint newMousePosition = mouseEvent->pos();
    QPoint moving = newMousePosition - _lastMousePosition;
    glm::vec2 relativeMoving = glm::vec2(moving.x(), moving.y()) /
                                  glm::vec2(_widget.width(), _widget.height());

    switch (_lastPressedButton)
    {
      case Qt::LeftButton:
        _rotate(relativeMoving);
        break;
      case Qt::RightButton:
        _moveHorizontal(relativeMoving);
        break;
      case Qt::MiddleButton:
        _moveVertical(relativeMoving);
        break;
    }

    _lastMousePosition = newMousePosition;
  }

  if(event->type() == QEvent::Wheel)
  {
    QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
    _approach(wheelEvent->angleDelta());
  }

  return QObject::eventFilter(obj, event);
}

void OrbitalCameraController::_rotate(const glm::vec2& mouseMoving) noexcept
{
  _zAngle -= mouseMoving.x * _rotateSensitivity;
  if(_zAngle > glm::pi<float>()) _zAngle -= 2 * glm::pi<float>();
  if(_zAngle < -glm::pi<float>()) _zAngle += 2.f * glm::pi<float>();

  _yAngle -= mouseMoving.y * _rotateSensitivity;
  _yAngle = glm::clamp(_yAngle, -glm::pi<float>() / 2.f, glm::pi<float>() / 2.f);
  
  _updateCameraPosition();
}

glm::vec3 OrbitalCameraController::_frontDirection() const noexcept
{
  return glm::vec3(-glm::cos(_zAngle), -glm::sin(_zAngle), 0);
}

glm::vec3 OrbitalCameraController::_rightDirection() const noexcept
{
  return glm::vec3(-glm::sin(_zAngle), glm::cos(_zAngle), 0);
}

float OrbitalCameraController::_movingRate() const noexcept
{
  if(_widgetCamera == nullptr) return 0;
  float probeDistance = glm::clamp( _distance,
                                    _widgetCamera->nearDistance(),
                                    _widgetCamera->farDistance());
  glm::vec4 centerPoint(0, 0, -probeDistance, 1);
  glm::vec4 projectedCenter = _widgetCamera->projectionMatrix() * centerPoint;
  projectedCenter /= projectedCenter.w;

  glm::vec4 projectedLeftPoint(-1, 0, projectedCenter.z, 1);
  glm::vec4 leftPointUnprojected =
                _widgetCamera->inverseProjectionMatrix() * projectedLeftPoint;
  glm::vec3 leftPoint = leftPointUnprojected / leftPointUnprojected.w;

  glm::vec4 projectedRightPoint(1, 0, projectedCenter.z, 1);
  glm::vec4 rightPointUnprojected =
                _widgetCamera->inverseProjectionMatrix() * projectedRightPoint;
  glm::vec3 rightPoint = rightPointUnprojected / rightPointUnprojected.w;

  return glm::length(leftPoint - rightPoint) * _movingSensitivity;
}

void OrbitalCameraController::_moveHorizontal(
                                          const glm::vec2& mouseMoving) noexcept
{
  glm::vec3 shift = -mouseMoving.x * _rightDirection() +
                                              mouseMoving.y * _frontDirection();
  _centerPosition += shift * _movingRate();
  _updateCameraPosition();
}

void OrbitalCameraController::_moveVertical(
                                          const glm::vec2& mouseMoving) noexcept
{
  glm::vec3 shift = -mouseMoving.x * _rightDirection() +
                                      mouseMoving.y * glm::vec3(0.f, 0.f, 1.f);
  _centerPosition += shift * _movingRate();
  _updateCameraPosition();
}

void OrbitalCameraController::_approach(const QPoint& angleDelta) noexcept
{
  if(_widgetCamera == nullptr) return;

  float steps = -angleDelta.y() * _approachSensitivity;
  float factor = glm::pow(1.1f, steps);
  _distance = glm::clamp( _distance * factor,
                          _widgetCamera->nearDistance(),
                          _widgetCamera->farDistance());
  _updateCameraPosition();
}

void OrbitalCameraController::_findCenterByClick(
                                              QMouseEvent& mouseEvent) noexcept
{
  if(_depthRenderer == nullptr) return;
  if(_widgetCamera == nullptr) return;
  if(_renderScene == nullptr) return;

  try
  {
    _depthRenderCamera.setTransformMatrix(_widgetCamera->transformMatrix());

    glm::mat4 projectionMatrix = _widgetCamera->projectionMatrix();

    glm::vec3 projectionScale(_widget.width(), _widget.height(), 1);
    projectionMatrix = glm::scale(projectionScale) * projectionMatrix;

    glm::vec2 center = glm::vec2(_widget.width(), _widget.height()) / 2.f;
    glm::vec2 mousePos(mouseEvent.pos().x(), mouseEvent.pos().y());
    mousePos += glm::vec2(.5f, .5f);
    glm::vec3 projectionShift((center - mousePos) * 2.f, 0);
    projectionMatrix = glm::translate(projectionShift) * projectionMatrix;

    _depthRenderCamera.setProjectionMatrix(projectionMatrix);

    glm::uvec2 rootExtent(_widget.width(), _widget.height());
    ViewInfo rootViewInfo = _widgetCamera->buildViewInfo(rootExtent);
    _depthRenderCamera.applyRootViewInfo( rootViewInfo,
                                          _widgetCamera->viewMatrix());

    glm::mat4 viewMatrix = _depthRenderCamera.viewMatrix();

    auto callback = [&, projectionMatrix, viewMatrix](void* data)
    {
      float* distance = (float*)data;
      if(*distance == 0) return;
      glm::vec4 projectedPoint(0, 0, *distance, 1);
      glm::mat4 unprojectMatrix = glm::inverse(projectionMatrix * viewMatrix);
      glm::vec4 unprojectedPoint = unprojectMatrix * projectedPoint;
      unprojectedPoint /= unprojectedPoint.w;
      _setCenter(unprojectedPoint);
    };

    std::unique_ptr<RenderImageAction> action(
                                      new RenderImageAction(
                                                      *_depthRenderer,
                                                      *_renderScene,
                                                      _depthRenderCamera,
                                                      &rootViewInfo,
                                                      glm::uvec2(1,1),
                                                      VK_IMAGE_ASPECT_DEPTH_BIT,
                                                      callback));
    Application& application = Application::instance();
    application.workCycle.addAction(std::move(action),
                                    WorkCycle::RENDER_CATEGORY,
                                    WorkCycle::RENDER_PRIORITY,
                                    false);
  }
  catch(std::exception& error)
  {
    Log() << "ClickSelectHandler::_processClick: " << error.what();
  }
  catch(...)
  {
    Log() << "ClickSelectHandler::_processClick: unknown error.";
  }
}

void OrbitalCameraController::_setCenter(glm::vec3 newCenter) noexcept
{
  if(_widgetCamera == nullptr) return;

  _distance = glm::length(_widgetCamera->eyePoint() - newCenter);
  _centerPosition = newCenter;
  _updateCameraPosition();
}
