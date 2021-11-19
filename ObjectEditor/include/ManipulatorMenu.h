#pragma once

#include <QtCore/QObject>

#include <Manipulator/EditorManipulatorController.h>

class MainWindow;

class Ui_MainWindow;

class ManipulatorMenu : public QObject
{
  Q_OBJECT

public:
  ManipulatorMenu(MainWindow& window,
                  EditorManipulatorController& controller,
                  Ui_MainWindow& ui);
  ManipulatorMenu(const ManipulatorMenu&) = delete;
  ManipulatorMenu& operator = (const ManipulatorMenu&) = delete;
  virtual ~ManipulatorMenu() noexcept = default;

  void setupUI();

private:
  void _setMoveManipulator() noexcept;
  void _setRotationManipulator() noexcept;
  void _setScaleManipulator() noexcept;
  void _setManipulatorType(
                  EditorManipulatorController::ManipulatorType type) noexcept;
  void _updateMenuFromManipulatorType() noexcept;

  void _setLocalOrientation() noexcept;
  void _setGlobalOrientation() noexcept;
  void _updateMenuFromOrientation() noexcept;

private:
  MainWindow& _window;
  EditorManipulatorController& _controller;
  Ui_MainWindow& _ui;
  bool _skipUpdate;
};