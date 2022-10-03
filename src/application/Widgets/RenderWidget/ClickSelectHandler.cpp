#include <stdexcept>

#include <QtGui/QMouseEvent>

#include <glm/gtx/transform.hpp>

#include <mtt/application/Scene/Scene.h>
#include <mtt/application/Widgets/RenderWidget/ClickSelectHandler.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/WorkCycle/RenderImageAction.h>
#include <mtt/application/Application.h>
#include <mtt/application/CommonEditData.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/RenderScene.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

ClickSelectHandler::ClickSelectHandler( RenderWidget& widget,
                                        SceneToImageRenderer& uidRenderer,
                                        CommonEditData& commonData) :
  _widget(widget),
  _sceneRenderer(uidRenderer),
  _widgetCamera(nullptr),
  _renderScene(nullptr),
  _commonData(commonData)
{
  widget.renderWindow().installEventFilter(this);

  connect(&_widget,
          &RenderWidget::cameraChanged,
          this,
          &ClickSelectHandler::_setCamera,
          Qt::DirectConnection);
  _setCamera(_widget.camera());

  connect(&_widget,
          &RenderWidget::sceneChanged,
          this,
          &ClickSelectHandler::_setRenderScene,
          Qt::DirectConnection);
  _setRenderScene(_widget.scene());
}

void ClickSelectHandler::_setCamera(CameraNode* camera) noexcept
{
  _widgetCamera = camera;
}

void ClickSelectHandler::_setRenderScene(RenderScene* newScene) noexcept
{
  _renderScene = newScene;
}

bool ClickSelectHandler::eventFilter(QObject* obj, QEvent* event)
{
  if(_widgetCamera != nullptr && _renderScene != nullptr)
  {
    if(event->type() == QEvent::MouseButtonPress)
    {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      if(mouseEvent->button() == Qt::LeftButton)
      {
        _clickMousePosition = mouseEvent->pos();
      }
    }

    if(event->type() == QEvent::MouseButtonRelease)
    {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      if (mouseEvent->button() == Qt::LeftButton &&
          _clickMousePosition == mouseEvent->pos())
      {
        _processClick(*mouseEvent);
      }
    }
  }

  return QObject::eventFilter(obj, event);
}

void ClickSelectHandler::_processClick(QMouseEvent& mouseEvent) noexcept
{
  try
  {
    _renderCamera.setTransformMatrix(_widgetCamera->transformMatrix());

    glm::mat4 projectionMatrix = _widgetCamera->projectionMatrix();

    glm::vec3 projectionScale(_widget.width(), _widget.height(), 1);
    projectionMatrix = glm::scale(projectionScale) * projectionMatrix;

    glm::vec2 center = glm::vec2(_widget.width(), _widget.height()) / 2.f;
    glm::vec2 mousePos(_clickMousePosition.x(), _clickMousePosition.y());
    mousePos += glm::vec2(.5f, .5f);
    glm::vec3 projectionShift((center - mousePos) * 2.f, 0);
    projectionMatrix = glm::translate(projectionShift) * projectionMatrix;

    _renderCamera.setProjectionMatrix(projectionMatrix);

    glm::uvec2 rootExtent(_widget.width(), _widget.height());
    ViewInfo rootViewInfo = _widgetCamera->buildViewInfo(rootExtent);
    _renderCamera.applyRootViewInfo(rootViewInfo, _widgetCamera->viewMatrix());

    Qt::KeyboardModifiers modifiers = mouseEvent.modifiers();
    auto callback = [&, modifiers](void* data)
    {
      glm::uvec2* rawData = (glm::uvec2*)data;
      uint64_t uidValue = rawData->x;
      uidValue += uint64_t(rawData->y) * 0x100000000;
      processClickedUid(UID(uidValue), modifiers);
    };

    std::unique_ptr<RenderImageAction> action(
                                          new RenderImageAction(
                                                      _sceneRenderer,
                                                      *_renderScene,
                                                      _renderCamera,
                                                      &rootViewInfo,
                                                      glm::uvec2(1,1),
                                                      VK_IMAGE_ASPECT_COLOR_BIT,
                                                      callback));
    Application& application = Application::instance();
    application.workCycle.addAction(std::move(action),
                                    RenderImageAction::actionCategory,
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

void ClickSelectHandler::processClickedUid(
                                      const UID& uid,
                                      Qt::KeyboardModifiers modifiers) noexcept
{
  try
  {
    Scene* scene = _commonData.scene();
    if(scene == nullptr) return;

    if(modifiers & Qt::ControlModifier)
    {
      Object* object = scene->findObject(uid);
      if(object != nullptr)
      {
        std::vector<Object*> selection = _commonData.selectedObjects();
        selection.insert(selection.begin(), object);
        _commonData.selectObjects(selection);
      }
    }
    else
    {
      Object* object = scene->findObject(uid);
      if(object == nullptr) _commonData.selectObjects(std::vector<Object*>());
      else _commonData.selectObjects({object});
    }
  }
  catch (std::exception& error)
  {
    Log() << "ClickSelectHandler::processClickedUid: " << error.what();
  }
  catch (...)
  {
    Log() << "ClickSelectHandler::processClickedUid: unknown error.";
  }
}
