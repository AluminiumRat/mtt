#include <stdexcept>

#include <QtWidgets/QMessageBox>

#include <mtt/utilities/ScopedSetter.h>

#include <ManipulatorMenu.h>
#include <MainWindow.h>

#include <GeneratedFiles/ui_MainWindow.h>

ManipulatorMenu::ManipulatorMenu( MainWindow& window,
                                  mtt::EditorManipulatorController& controller,
                                  Ui_MainWindow& ui) :
  _window(window),
  _controller(controller),
  _ui(ui),
  _skipUpdate(false)
{
}

void ManipulatorMenu::setupUI()
{
  connect(&_controller,
          &mtt::EditorManipulatorController::currentManipulatorChanged,
          this,
          &ManipulatorMenu::_updateMenuFromManipulatorType,
          Qt::DirectConnection);
  _updateMenuFromManipulatorType();

  connect(_ui.actionSetMoveManipulator,
          &QAction::toggled,
          this,
          &ManipulatorMenu::_setMoveManipulator,
          Qt::DirectConnection);
  
  connect(_ui.actionSetRotationManipulator,
          &QAction::toggled,
          this,
          &ManipulatorMenu::_setRotationManipulator,
          Qt::DirectConnection);

  connect(_ui.actionSetScaleManipulator,
          &QAction::toggled,
          this,
          &ManipulatorMenu::_setScaleManipulator,
          Qt::DirectConnection);

  connect(&_controller,
          &mtt::EditorManipulatorController::orientationChanged,
          this,
          &ManipulatorMenu::_updateMenuFromOrientation,
          Qt::DirectConnection);
  _updateMenuFromOrientation();

  connect(_ui.actionSetLocalOrientation,
          &QAction::toggled,
          this,
          &ManipulatorMenu::_setLocalOrientation,
          Qt::DirectConnection);

  connect(_ui.actionSetGlobalOrientation,
          &QAction::toggled,
          this,
          &ManipulatorMenu::_setGlobalOrientation,
          Qt::DirectConnection);
}

void ManipulatorMenu::_setMoveManipulator() noexcept
{
  if (_skipUpdate) return;
  mtt::ScopedTrueSetter lock(_skipUpdate);
  _ui.actionSetMoveManipulator->setChecked(true);
  _ui.actionSetRotationManipulator->setChecked(false);
  _ui.actionSetScaleManipulator->setChecked(false);
  _setManipulatorType(mtt::EditorManipulatorController::MOVE_MANIPULATOR);
}

void ManipulatorMenu::_setRotationManipulator() noexcept
{
  if (_skipUpdate) return;
  mtt::ScopedTrueSetter lock(_skipUpdate);
  _ui.actionSetMoveManipulator->setChecked(false);
  _ui.actionSetRotationManipulator->setChecked(true);
  _ui.actionSetScaleManipulator->setChecked(false);
  _setManipulatorType(mtt::EditorManipulatorController::ROTATION_MANIPULATOR);
}

void ManipulatorMenu::_setScaleManipulator() noexcept
{
  if (_skipUpdate) return;
  mtt::ScopedTrueSetter lock(_skipUpdate);
  _ui.actionSetMoveManipulator->setChecked(false);
  _ui.actionSetRotationManipulator->setChecked(false);
  _ui.actionSetScaleManipulator->setChecked(true);
  _setManipulatorType(mtt::EditorManipulatorController::SCALE_MANIPULATOR);
}

void ManipulatorMenu::_setManipulatorType(
              mtt::EditorManipulatorController::ManipulatorType type) noexcept
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
  mtt::ScopedTrueSetter lock(_skipUpdate);

  _ui.actionSetMoveManipulator->setChecked(
                  _controller.currentManipulator() ==
                            mtt::EditorManipulatorController::MOVE_MANIPULATOR);
  _ui.actionSetRotationManipulator->setChecked(
                  _controller.currentManipulator() ==
                        mtt::EditorManipulatorController::ROTATION_MANIPULATOR);
  _ui.actionSetScaleManipulator->setChecked(
                  _controller.currentManipulator() ==
                          mtt::EditorManipulatorController::SCALE_MANIPULATOR);
}

void ManipulatorMenu::_setLocalOrientation() noexcept
{
  if (_skipUpdate) return;
  mtt::ScopedTrueSetter lock(_skipUpdate);

  _ui.actionSetLocalOrientation->setChecked(true);
  _ui.actionSetGlobalOrientation->setChecked(false);
  _controller.setOrientation(mtt::ObjectManipulator::LOCAL_ORIENTATION);
}

void ManipulatorMenu::_setGlobalOrientation() noexcept
{
  if (_skipUpdate) return;
  mtt::ScopedTrueSetter lock(_skipUpdate);

  _ui.actionSetLocalOrientation->setChecked(false);
  _ui.actionSetGlobalOrientation->setChecked(true);
  _controller.setOrientation(mtt::ObjectManipulator::GLOBAL_ORIENTATION);
}

void ManipulatorMenu::_updateMenuFromOrientation() noexcept
{
  if (_skipUpdate) return;
  mtt::ScopedTrueSetter lock(_skipUpdate);

  _ui.actionSetLocalOrientation->setChecked(
                          _controller.orientation() ==
                                    mtt::ObjectManipulator::LOCAL_ORIENTATION);
  _ui.actionSetGlobalOrientation->setChecked(
                          _controller.orientation() ==
                                    mtt::ObjectManipulator::GLOBAL_ORIENTATION);
}
