#include <QtGui/QMouseEvent>

#include <mtt/application/Application.h>

#include <RenderScene.h>
#include <RenderWindow.h>
#include <World.h>

RenderWindow::RenderWindow(World& world) :
  _cameraController(*this, nullptr),
  _frameBuilder(surfaceFormat(),
                mtt::Application::instance().displayDevice()),
  _world(world),
  _rotationDistribution(-glm::pi<float>(), glm::pi<float>()),
  _sizeDistribution(.1f, 2.f)
{
  renderWindow().installEventFilter(this);

  _cameraController.setCenterPosition(glm::vec3(0.f, 0.f, 5.f));
  _cameraController.setDistance(20.f);
  _cameraController.setZAngle(-glm::pi<float>() / 2.f);
  _cameraController.setYAngle(-.5f);

  _camera.setPerspectiveProjection(glm::pi<float>() / 4.f, 1.33f, 0.1f, 200.f);
  setSource(&_world.renderScene(), &_camera);

  setFrameBuilder(&_frameBuilder);
}

RenderWindow::~RenderWindow() noexcept
{
  setSource(nullptr, nullptr);
  setFrameBuilder(nullptr);
}

bool RenderWindow::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::MouseButtonDblClick)
  {
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

    std::optional<glm::vec2> insertPosition = getInsertPosition(*mouseEvent);
    if(insertPosition.has_value())
    {
      if (mouseEvent->button() == Qt::LeftButton)
      {
        //Add random sphere
        float radius = _sizeDistribution(_randomEngine) / 2.f;
        float volume = 4.f / 3.f * glm::pi<float>() * radius * radius * radius;
        float mass = 600 * volume;
        _world.addSphere(*insertPosition, radius, 1.f / mass);
      }
      else if (mouseEvent->button() == Qt::RightButton)
      {
        //Add random box
        float rotation = _rotationDistribution(_randomEngine);
        glm::vec2 size( _sizeDistribution(_randomEngine),
                        _sizeDistribution(_randomEngine));
        float volume = size.x * size.y * (size.x + size.y) / 2.f;
        float mass = 600 * volume;
        _world.addBox(*insertPosition, rotation, size, 1.f / mass);
      }
    }
  }

  return RenderWidget::eventFilter(obj, event);
}

std::optional<glm::vec2> RenderWindow::getInsertPosition(
                                        QMouseEvent& mouseEvent) const noexcept
{
  if (camera() == nullptr ||
      renderWindow().width() == 0 ||
      renderWindow().height() == 0) return std::optional<glm::vec2>();

  glm::vec3 cameraPosition =
                    camera()->transformMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f);

  //Convert Qt window coordinates the mtt screen space
  glm::vec2 mousePosition(mouseEvent.windowPos().x(),
                          mouseEvent.windowPos().y());
  mousePosition /= glm::vec2( renderWindow().width(),
                              renderWindow().height());
  mousePosition *= 2;
  mousePosition -= glm::vec2(1, 1);

  //Mouse click ray
  glm::vec3 touchDirection = camera()->getDirection(mousePosition);
  if(touchDirection.y == 0.f) return std::optional<glm::vec2>();

  float factor = -cameraPosition.y / touchDirection.y;
  if(factor < 0.f) return std::optional<glm::vec2>();

  glm::vec3 projected = cameraPosition + touchDirection * factor;
  if( projected.x > _world.size().x / 2.f ||
      projected.x < -_world.size().x / 2.f ||
      projected.y > _world.size().y / 2.f ||
      projected.y < -_world.size().y / 2.f)
  {
    return std::optional<glm::vec2>();
  }
  return glm::vec2(projected.x, projected.z);
}
