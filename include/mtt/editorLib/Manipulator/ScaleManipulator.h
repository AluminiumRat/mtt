#pragma once

#include <mtt/editorLib/Manipulator/ObjectManipulator.h>
#include <mtt/editorLib/Manipulator/TriangleManipulator.h>
#include <mtt/render/SceneGraph/Joint.h>

namespace mtt
{
  class UndoStack;

  class ScaleManipulator : public ObjectManipulator
  {
  public:
    ScaleManipulator(ScalableObject& object, UndoStack& undoStack);
    ScaleManipulator(const ScaleManipulator&) = delete;
    ScaleManipulator& operator = (const ScaleManipulator&) = delete;
    virtual ~ScaleManipulator() noexcept = default;

  private:
    Joint _xManipulatorJoint;
    TriangleManipulator _xPlaneManipulator;

    Joint _yManipulatorJoint;
    TriangleManipulator _yPlaneManipulator;

    TriangleManipulator _zPlaneManipulator;
  };
}