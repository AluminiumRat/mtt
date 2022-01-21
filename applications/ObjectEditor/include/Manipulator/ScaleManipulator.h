#pragma once

#include <mtt/render/SceneGraph/Joint.h>

#include <Manipulator/ObjectManipulator.h>
#include <Manipulator/TriangleManipulator.h>

namespace mtt
{
  class UndoStack;
}

class ScaleManipulator : public ObjectManipulator
{
public:
  ScaleManipulator(ScalableObject& object, mtt::UndoStack& undoStack);
  ScaleManipulator(const ScaleManipulator&) = delete;
  ScaleManipulator& operator = (const ScaleManipulator&) = delete;
  virtual ~ScaleManipulator() noexcept = default;

private:
  mtt::Joint _xManipulatorJoint;
  TriangleManipulator _xPlaneManipulator;

  mtt::Joint _yManipulatorJoint;
  TriangleManipulator _yPlaneManipulator;

  TriangleManipulator _zPlaneManipulator;
};