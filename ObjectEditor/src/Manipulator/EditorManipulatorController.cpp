#include <Manipulator/EditorManipulatorController.h>
#include <RenderWidget/EditorRenderWidget.h>
#include <Objects/MovableObject.h>
#include <Objects/RotatableObject.h>
#include <Objects/ScalableObject.h>
#include <EditorCommonData.h>

EditorManipulatorController::EditorManipulatorController(
                                                EditorRenderWidget& widget,
                                                EditorCommonData& commonData) :
  ManipulatorController(widget),
  _widget(widget),
  _commonData(commonData),
  _currentManipulator(MOVE_MANIPULATOR),
  _orientation(ObjectManipulator::LOCAL_ORIENTATION)
{
  connect(&commonData,
          &EditorCommonData::selectedObjectsChanged,
          this,
          &EditorManipulatorController::_updateManipulator,
          Qt::DirectConnection);
  _updateManipulator();
}

EditorManipulatorController::~EditorManipulatorController() noexcept
{
  resetManipulator();
}

void EditorManipulatorController::_updateManipulator()
{
  _switchManipulator(NO_MANIPULATOR);
  _moveManipulator.reset();
  _rotationManipulator.reset();
  _scaleManipulator.reset();

  if(_commonData.selectedObjects().size() == 1)
  {
    MovableObject* movable =
                qobject_cast<MovableObject*>(_commonData.selectedObjects()[0]);
    if(movable != nullptr)
    {
      _moveManipulator.emplace(*movable, _commonData.undoStack());
    }

    RotatableObject* rotatable =
              qobject_cast<RotatableObject*>(_commonData.selectedObjects()[0]);
    if(rotatable != nullptr)
    {
      _rotationManipulator.emplace(*rotatable, _commonData.undoStack());
    }

    ScalableObject* scalable =
              qobject_cast<ScalableObject*>(_commonData.selectedObjects()[0]);
    if(scalable != nullptr)
    {
      _scaleManipulator.emplace(*scalable, _commonData.undoStack());
    }

    _switchManipulator(_currentManipulator);
    _updateOrientation();
  }
}

void EditorManipulatorController::_switchManipulator(ManipulatorType type)
{
  switch(type)
  {
    case NO_MANIPULATOR:
      resetManipulator();
      break;

    case MOVE_MANIPULATOR:
      if(!_moveManipulator.has_value()) resetManipulator();
      else setManipulator(&_moveManipulator.value());
      break;

    case ROTATION_MANIPULATOR:
      if(!_rotationManipulator.has_value()) resetManipulator();
      else setManipulator(&_rotationManipulator.value());
      break;

    case SCALE_MANIPULATOR:
      if(!_scaleManipulator.has_value()) resetManipulator();
      else setManipulator(&_scaleManipulator.value());
      break;
  };
}

void EditorManipulatorController::setCurrentManipulator(
                                                ManipulatorType newManipulator)
{
  if(newManipulator == _currentManipulator) return;

  try
  {
    _switchManipulator(newManipulator);
  }
  catch(...)
  {
    resetManipulator();
    _currentManipulator = NO_MANIPULATOR;
    emit currentManipulatorChanged(_currentManipulator);
    throw;
  }
  
  _currentManipulator = newManipulator;
  emit currentManipulatorChanged(_currentManipulator);
}

void EditorManipulatorController::_updateOrientation() noexcept
{
  if(_moveManipulator.has_value())
  {
    _moveManipulator->setOrientation(_orientation);
  }

  if(_rotationManipulator.has_value())
  {
    _rotationManipulator->setOrientation(_orientation);
  }

  if(_scaleManipulator.has_value())
  {
    _scaleManipulator->setOrientation(_orientation);
  }
}

void EditorManipulatorController::setOrientation(
                              ObjectManipulator::Orientation newValue) noexcept
{
  if(newValue == _orientation) return;
  _orientation = newValue;
  _updateOrientation();
  emit orientationChanged(_orientation);
}
