#pragma once

#include <mtt/render/SceneGraph/Joint.h>

#include <Manipulator/AxisManipulator.h>
#include <Manipulator/ObjectManipulator.h>
#include <Manipulator/PlaneManipulator.h>

namespace mtt
{
  class UndoStack;
}

class MoveManipulator : public ObjectManipulator
{
public:
  MoveManipulator(MovableObject& object, mtt::UndoStack& undoStack);
  MoveManipulator(const MoveManipulator&) = delete;
  MoveManipulator& operator = (const MoveManipulator&) = delete;
  virtual ~MoveManipulator() noexcept = default;

private:
  mtt::Joint _xManipulatorJoint;
  AxisManipulator _xAxisManipulator;
  PlaneManipulator _xPlaneManipulator;

  mtt::Joint _yManipulatorJoint;
  AxisManipulator _yAxisManipulator;
  PlaneManipulator _yPlaneManipulator;

  AxisManipulator _zAxisManipulator;
  PlaneManipulator _zPlaneManipulator;
};