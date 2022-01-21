#pragma once

#include <optional>

#include <mtt/application/Manipulator/ManipulatorController.h>

#include <Manipulator/MoveManipulator.h>
#include <Manipulator/RotationManipulator.h>
#include <Manipulator/ScaleManipulator.h>

class EditorCommonData;
class EditorRenderWidget;

class EditorManipulatorController : public mtt::ManipulatorController
{
  Q_OBJECT

public:
  enum ManipulatorType
  {
    NO_MANIPULATOR,
    MOVE_MANIPULATOR,
    ROTATION_MANIPULATOR,
    SCALE_MANIPULATOR
  };

public:
  EditorManipulatorController(EditorRenderWidget& widget,
                              EditorCommonData& commonData);
  EditorManipulatorController(const EditorManipulatorController&) = delete;
  EditorManipulatorController& operator = (
                                  const EditorManipulatorController&) = delete;
  virtual ~EditorManipulatorController() noexcept;

  inline ManipulatorType currentManipulator() const noexcept;
  void setCurrentManipulator(ManipulatorType newManipulator);

  inline ObjectManipulator::Orientation orientation() const noexcept;
  void setOrientation(ObjectManipulator::Orientation newValue) noexcept;

signals:
  void currentManipulatorChanged(ManipulatorType newManipulator);
  void orientationChanged(ObjectManipulator::Orientation newValue);

private:
  void _updateManipulator();
  void _switchManipulator(ManipulatorType type);
  void _updateOrientation() noexcept;

private:
  EditorRenderWidget& _widget;
  EditorCommonData& _commonData;

  ManipulatorType _currentManipulator;
  ObjectManipulator::Orientation _orientation;

  std::optional<MoveManipulator> _moveManipulator;
  std::optional<RotationManipulator> _rotationManipulator;
  std::optional<ScaleManipulator> _scaleManipulator;
};

inline EditorManipulatorController::ManipulatorType
                EditorManipulatorController::currentManipulator() const noexcept
{
  return _currentManipulator;
}

inline ObjectManipulator::Orientation
                      EditorManipulatorController::orientation() const noexcept
{
  return _orientation;
}
