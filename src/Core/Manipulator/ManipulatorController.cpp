#include <stdexcept>

#include <QtGui/QMouseEvent>

#include <mtt/Core/Manipulator/ManipulatorController.h>
#include <mtt/Core/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/Render/SceneGraph/CameraNode.h>
#include <mtt/Render/RenderScene.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

ManipulatorController::ManipulatorController(RenderWidget& widget) :
  _widget(widget),
  _camera(nullptr),
  _renderScene(nullptr),
  _currentManipulator(nullptr),
  _manipulatorDrawable(nullptr)
{
  widget.renderWindow().installEventFilter(this);

  connect(&_widget,
          &RenderWidget::cameraChanged,
          this,
          &ManipulatorController::_setCamera,
          Qt::DirectConnection);
  _setCamera(_widget.camera());

  connect(&_widget,
          &RenderWidget::sceneChanged,
          this,
          &ManipulatorController::_setRenderScene,
          Qt::DirectConnection);
  _setRenderScene(_widget.scene());
}

ManipulatorController::~ManipulatorController() noexcept
{
  resetManipulator();
}

void ManipulatorController::_setCamera(CameraNode* camera) noexcept
{
  _camera = camera;
}

void ManipulatorController::_setRenderScene(RenderScene* newScene) noexcept
{
  if(_renderScene == newScene) return;

  if (_manipulatorDrawable != nullptr && _renderScene != nullptr)
  {
    _renderScene->unculledData().removeChild(*_manipulatorDrawable);
  }

  _renderScene = newScene;

  try
  {
    if (_manipulatorDrawable != nullptr && _renderScene != nullptr)
    {
      _renderScene->unculledData().addChild(*_manipulatorDrawable);
    }
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("ManipulatorController::_setRenderScene: unable to add manipulator drawable to render scene.");
  }
  catch (...)
  {
    Abort("ManipulatorController::_setRenderScene: unknown error.");
  }
}

void ManipulatorController::resetManipulator() noexcept
{
  try
  {
    if (_manipulatorDrawable != nullptr && _renderScene != nullptr)
    {
      _renderScene->unculledData().removeChild(*_manipulatorDrawable);
    }

    if(_currentManipulator != nullptr)
    {
      if(_currentManipulator->activated())_currentManipulator->release();
      _currentManipulator->setHighlighted(false);
    }

    _currentManipulator = nullptr;
  }
  catch(std::exception& error)
  {
    Log() << error.what();
    Abort("ManipulatorController::_resetManipulator: unable to reset manipulator.");
  }
  catch(...)
  {
    Abort("ManipulatorController::_resetManipulator: unknown error.");
  }
}

void ManipulatorController::setManipulator(AbstractManipulator* newManipulator)
{
  if(_currentManipulator == newManipulator) return;

  resetManipulator();

  try
  {
    _currentManipulator = newManipulator;
    if(_currentManipulator != nullptr)
    {
      _manipulatorDrawable = &_currentManipulator->drawable();
      if(_renderScene != nullptr)
      {
        _renderScene->unculledData().addChild(*_manipulatorDrawable);
      }
    }
  }
  catch(...)
  {
    resetManipulator();
    throw;
  }
}

bool ManipulatorController::eventFilter(QObject* obj, QEvent* event)
{
  if(_currentManipulator != nullptr)
  {
    if(event->type() == QEvent::MouseButtonPress)
    {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      if( mouseEvent->button() == Qt::LeftButton &&
          _camera != nullptr &&
          _widget.width() != 0 &&
          _widget.height() != 0)
      {
        AbstractManipulator::TouchInfo touchInfo = _touchInfo(*mouseEvent);
        if(_currentManipulator->tryActivate(touchInfo).has_value())
        {
          return true;
        }
      }
    }    
  
    if(event->type() == QEvent::MouseButtonRelease)
    {
      if(_currentManipulator->activated())
      {
        _currentManipulator->release();
        return true;
      }
    }
  
    if( event->type() == QEvent::MouseMove &&
        _camera != nullptr &&
        _widget.width() != 0 &&
        _widget.height() != 0)
    {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      AbstractManipulator::TouchInfo touchInfo = _touchInfo(*mouseEvent);

      if(_currentManipulator->checkTouchPosition(touchInfo) != 0)
      {
        _currentManipulator->setHighlighted(true);
      }
      else _currentManipulator->setHighlighted(false);

      _currentManipulator->processTouchMove(touchInfo);
    }
  }

  return QObject::eventFilter(obj, event);
}

AbstractManipulator::TouchInfo ManipulatorController::_touchInfo(
                                        QMouseEvent& mouseEvent) const noexcept
{
  glm::vec2 mousePosition(mouseEvent.windowPos().x(),
                          mouseEvent.windowPos().y());
  mousePosition /= glm::vec2( _widget.renderWindow().width(),
                              _widget.renderWindow().height());
  mousePosition *= 2;
  mousePosition -= glm::vec2(1, 1);

  return AbstractManipulator::TouchInfo{
    glm::uvec2(mouseEvent.windowPos().x(), mouseEvent.windowPos().y()),
    glm::uvec2(_widget.renderWindow().width(),
               _widget.renderWindow().height()),
               mousePosition,
               *_camera};
}
