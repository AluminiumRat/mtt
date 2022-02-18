#pragma once

#include <mtt/editorLib/Manipulator/ObjectManipulator.h>
#include <mtt/editorLib/Manipulator/RingManipulator.h>
#include <mtt/render/SceneGraph/Joint.h>

namespace mtt
{
  class UndoStack;

  class RotationManipulator : public ObjectManipulator
  {
  public:
    RotationManipulator(RotatableObject& object, UndoStack& undoStack);
    RotationManipulator(const RotationManipulator&) = delete;
    RotationManipulator& operator = (const RotationManipulator&) = delete;
    virtual ~RotationManipulator() noexcept = default;

  private:
    Joint _xManipulatorJoint;
    RingManipulator _xManipulator;

    Joint _yManipulatorJoint;
    RingManipulator _yManipulator;

    RingManipulator _zManipulator;
  };
}