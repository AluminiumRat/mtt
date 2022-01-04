#pragma once

#include <mtt/Render/SceneGraph/Joint.h>

#include <Manipulator/ObjectManipulator.h>
#include <Manipulator/RingManipulator.h>

namespace mtt
{
  class UndoStack;
}

class RotationManipulator : public ObjectManipulator
{
public:
  RotationManipulator(RotatableObject& object, mtt::UndoStack& undoStack);
  RotationManipulator(const RotationManipulator&) = delete;
  RotationManipulator& operator = (const RotationManipulator&) = delete;
  virtual ~RotationManipulator() noexcept = default;

private:
  mtt::Joint _xManipulatorJoint;
  RingManipulator _xManipulator;

  mtt::Joint _yManipulatorJoint;
  RingManipulator _yManipulator;

  RingManipulator _zManipulator;
};