#pragma once

#include <QtWidgets/QMenu>

#include <mtt/editorLib/Manipulator/EditorManipulatorController.h>

namespace mtt
{
  class ManipulatorMenu : public QMenu
  {
    Q_OBJECT

  public:
    ManipulatorMenu(QWidget& window, EditorManipulatorController& controller);
    ManipulatorMenu(const ManipulatorMenu&) = delete;
    ManipulatorMenu& operator = (const ManipulatorMenu&) = delete;
    virtual ~ManipulatorMenu() noexcept = default;

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
    QWidget& _window;
    EditorManipulatorController& _controller;
    bool _skipUpdate;

    QAction* _setMovingAction;
    QAction* _setRotationAction;
    QAction* _setScaleAction;

    QAction* _localOrientationAction;
    QAction* _globalOrientationAction;
  };
}