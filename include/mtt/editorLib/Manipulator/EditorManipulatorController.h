#pragma once

#include <optional>

#include <mtt/application/Manipulator/ManipulatorController.h>
#include <mtt/editorLib/Manipulator/MoveManipulator.h>
#include <mtt/editorLib/Manipulator/ObjectManipulator.h>
#include <mtt/editorLib/Manipulator/RotationManipulator.h>
#include <mtt/editorLib/Manipulator/ScaleManipulator.h>

namespace mtt
{
  class CommonEditData;

  class EditorManipulatorController : public ManipulatorController
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
    EditorManipulatorController(RenderWidget& widget,
                                CommonEditData& commonData);
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
    CommonEditData& _commonData;

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
}