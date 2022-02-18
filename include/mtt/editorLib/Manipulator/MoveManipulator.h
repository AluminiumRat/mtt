#pragma once

#include <mtt/editorLib/Manipulator/AxisManipulator.h>
#include <mtt/editorLib/Manipulator/ObjectManipulator.h>
#include <mtt/editorLib/Manipulator/PlaneManipulator.h>
#include <mtt/render/SceneGraph/Joint.h>

namespace mtt
{
  class UndoStack;

  class MoveManipulator : public ObjectManipulator
  {
  public:
    MoveManipulator(MovableObject& object, UndoStack& undoStack);
    MoveManipulator(const MoveManipulator&) = delete;
    MoveManipulator& operator = (const MoveManipulator&) = delete;
    virtual ~MoveManipulator() noexcept = default;

  private:
    Joint _xManipulatorJoint;
    AxisManipulator _xAxisManipulator;
    PlaneManipulator _xPlaneManipulator;

    Joint _yManipulatorJoint;
    AxisManipulator _yAxisManipulator;
    PlaneManipulator _yPlaneManipulator;

    AxisManipulator _zAxisManipulator;
    PlaneManipulator _zPlaneManipulator;
  };
}