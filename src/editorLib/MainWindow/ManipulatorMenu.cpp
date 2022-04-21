#include <stdexcept>

#include <QtWidgets/QMessageBox>

#include <mtt/editorLib/MainWindow/ManipulatorMenu.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

ManipulatorMenu::ManipulatorMenu( QWidget& window,
                                  EditorManipulatorController& controller) :
  QMenu(tr("&Manipulator")),
  _window(window),
  _controller(controller),
  _skipUpdate(false),
  _setMovingAction(nullptr),
  _setRotationAction(nullptr),
  _setScaleAction(nullptr),
  _localOrientationAction(nullptr),
  _globalOrientationAction(nullptr)
{
  _setMovingAction = addAction( tr("&Moving"),
                                this,
                                &ManipulatorMenu::_setMoveManipulator);
  _setMovingAction->setCheckable(true);
  _setMovingAction->setShortcut(Qt::Key_G);

  _setRotationAction = addAction( tr("&Rotation"),
                                  this,
                                  &ManipulatorMenu::_setRotationManipulator);
  _setRotationAction->setCheckable(true);
  _setRotationAction->setShortcut(Qt::Key_R);

  _setScaleAction = addAction(tr("&Scale"),
                              this,
                              &ManipulatorMenu::_setScaleManipulator);
  _setScaleAction->setCheckable(true);
  _setScaleAction->setShortcut(Qt::Key_S);

  _localOrientationAction = addAction(tr("&Local orienation"),
                                      this,
                                      &ManipulatorMenu::_setLocalOrientation);
  _localOrientationAction->setCheckable(true);
  _localOrientationAction->setShortcut(Qt::CTRL + Qt::Key_L);

  _globalOrientationAction = addAction(
                                      tr("&Global orienation"),
                                      this,
                                      &ManipulatorMenu::_setGlobalOrientation);
  _globalOrientationAction->setCheckable(true);
  _globalOrientationAction->setShortcut(Qt::CTRL + Qt::Key_G);

  connect(&_controller,
          &EditorManipulatorController::currentManipulatorChanged,
          this,
          &ManipulatorMenu::_updateMenuFromManipulatorType,
          Qt::DirectConnection);
  _updateMenuFromManipulatorType();

  connect(&_controller,
          &EditorManipulatorController::orientationChanged,
          this,
          &ManipulatorMenu::_updateMenuFromOrientation,
          Qt::DirectConnection);
  _updateMenuFromOrientation();
}

void ManipulatorMenu::_setMoveManipulator() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);
  _setMovingAction->setChecked(true);
  _setRotationAction->setChecked(false);
  _setScaleAction->setChecked(false);
  _setManipulatorType(EditorManipulatorController::MOVE_MANIPULATOR);
}

void ManipulatorMenu::_setRotationManipulator() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);
  _setMovingAction->setChecked(false);
  _setRotationAction->setChecked(true);
  _setScaleAction->setChecked(false);
  _setManipulatorType(EditorManipulatorController::ROTATION_MANIPULATOR);
}

void ManipulatorMenu::_setScaleManipulator() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);
  _setMovingAction->setChecked(false);
  _setRotationAction->setChecked(false);
  _setScaleAction->setChecked(true);
  _setManipulatorType(EditorManipulatorController::SCALE_MANIPULATOR);
}

void ManipulatorMenu::_setManipulatorType(
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

void ManipulatorMenu::_updateMenuFromManipulatorType() noexcept
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

void ManipulatorMenu::_setLocalOrientation() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);

  _localOrientationAction->setChecked(true);
  _globalOrientationAction->setChecked(false);
  _controller.setOrientation(ObjectManipulator::LOCAL_ORIENTATION);
}

void ManipulatorMenu::_setGlobalOrientation() noexcept
{
  if (_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);

  _localOrientationAction->setChecked(false);
  _globalOrientationAction->setChecked(true);
  _controller.setOrientation(ObjectManipulator::GLOBAL_ORIENTATION);
}

void ManipulatorMenu::_updateMenuFromOrientation() noexcept
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
