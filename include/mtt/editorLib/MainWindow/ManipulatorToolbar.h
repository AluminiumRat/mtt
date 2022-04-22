#pragma once

#include <QtWidgets/QToolBar>

#include <mtt/editorLib/Manipulator/EditorManipulatorController.h>

namespace mtt
{
  class ManipulatorToolbar : public QToolBar
  {
    Q_OBJECT

  public:
    ManipulatorToolbar( QWidget& window,
                        EditorManipulatorController& controller);
    ManipulatorToolbar(const ManipulatorToolbar&) = delete;
    ManipulatorToolbar& operator = (const ManipulatorToolbar&) = delete;
    virtual ~ManipulatorToolbar() noexcept = default;

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