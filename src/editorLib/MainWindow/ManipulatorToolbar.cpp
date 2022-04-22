#include <stdexcept>

#include <QtWidgets/QMessageBox>

#include <mtt/editorLib/MainWindow/ManipulatorToolbar.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

ManipulatorToolbar::ManipulatorToolbar(
                                      QWidget& window,
                                      EditorManipulatorController& controller) :
  _window(window),
  _controller(controller),
  _skipUpdate(false),
  _setMovingAction(nullptr),
  _setRotationAction(nullptr),
  _setScaleAction(nullptr),
  _localOrientationAction(nullptr),
  _globalOrientationAction(nullptr)
{
  _setMovingAction = addAction( QIcon(":/editorLib/movingGizmo.png"),
                                tr("Moving"),
                                this,
                                &ManipulatorToolbar::_setMoveManipulator);
  _setMovingAction->setCheckable(true);
  _setMovingAction->setShortcut(Qt::Key_G);

  _setRotationAction = addAction( QIcon(":/editorLib/rotateGizmo.png"),
                                  tr("Rotation"),
                                  this,
                                  &ManipulatorToolbar::_setRotationManipulator);
  _setRotationAction->setCheckable(true);
  _setRotationAction->setShortcut(Qt::Key_R);

  _setScaleAction = addAction(QIcon(":/editorLib/scaleGizmo.png"),
                              tr("Scale"),
                              this,
                              &ManipulatorToolbar::_setScaleManipulator);
  _setScaleAction->setCheckable(true);
  _setScaleAction->setShortcut(Qt::Key_S);

  _localOrientationAction = addAction(
                                    tr("Local"),
                                    this,
                                    &ManipulatorToolbar::_setLocalOrientation);
  _localOrientationAction->setCheckable(true);
  _localOrientationAction->setShortcut(Qt::CTRL + Qt::Key_L);

  _globalOrientationAction = addAction(
                                    tr("Global"),
                                    this,
                                    &ManipulatorToolbar::_setGlobalOrientation);
  _globalOrientationAction->setCheckable(true);
  _globalOrientationAction->setShortcut(Qt::CTRL + Qt::Key_G);

  connect(&_controller,
          &EditorManipulatorController::currentManipulatorChanged,
          this,
          &ManipulatorToolbar::_updateMenuFromManipulatorType,
          Qt::DirectConnection);
  _updateMenuFromManipulatorType();

  connect(&_controller,
          &EditorManipulatorController::orientationChanged,
          this,
          &ManipulatorToolbar::_updateMenuFromOrientation,
          Qt::DirectConnection);
  _updateMenuFromOrientation();
}

void ManipulatorToolbar::_setMoveManipulator() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);
  _setMovingAction->setChecked(true);
  _setRotationAction->setChecked(false);
  _setScaleAction->setChecked(false);
  _setManipulatorType(EditorManipulatorController::MOVE_MANIPULATOR);
}

void ManipulatorToolbar::_setRotationManipulator() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);
  _setMovingAction->setChecked(false);
  _setRotationAction->setChecked(true);
  _setScaleAction->setChecked(false);
  _setManipulatorType(EditorManipulatorController::ROTATION_MANIPULATOR);
}

void ManipulatorToolbar::_setScaleManipulator() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);
  _setMovingAction->setChecked(false);
  _setRotationAction->setChecked(false);
  _setScaleAction->setChecked(true);
  _setManipulatorType(EditorManipulatorController::SCALE_MANIPULATOR);
}

void ManipulatorToolbar::_setManipulatorType(
              EditorManipulatorController::ManipulatorType type) noexcept
{
  try
  {
    _controller.setCurrentManipulator(type);
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window, tr("Error"), error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unknown error"));
  }
}

void ManipulatorToolbar::_updateMenuFromManipulatorType() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);

  _setMovingAction->setChecked(
                  _controller.currentManipulator() ==
                                EditorManipulatorController::MOVE_MANIPULATOR);
  _setRotationAction->setChecked(
                  _controller.currentManipulator() ==
                            EditorManipulatorController::ROTATION_MANIPULATOR);
  _setScaleAction->setChecked(
                  _controller.currentManipulator() ==
                              EditorManipulatorController::SCALE_MANIPULATOR);
}

void ManipulatorToolbar::_setLocalOrientation() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);

  _localOrientationAction->setChecked(true);
  _globalOrientationAction->setChecked(false);
  _controller.setOrientation(ObjectManipulator::LOCAL_ORIENTATION);
}

void ManipulatorToolbar::_setGlobalOrientation() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);

  _localOrientationAction->setChecked(false);
  _globalOrientationAction->setChecked(true);
  _controller.setOrientation(ObjectManipulator::GLOBAL_ORIENTATION);
}

void ManipulatorToolbar::_updateMenuFromOrientation() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);

  _localOrientationAction->setChecked(
                          _controller.orientation() ==
                                        ObjectManipulator::LOCAL_ORIENTATION);
  _globalOrientationAction->setChecked(
                          _controller.orientation() ==
                                        ObjectManipulator::GLOBAL_ORIENTATION);
}
