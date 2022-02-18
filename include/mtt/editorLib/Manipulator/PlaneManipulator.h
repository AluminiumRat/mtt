#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Manipulator/PlaneMove3DManipulator.h>

namespace mtt
{
  class MovableObject;

  class PlaneManipulator : public PlaneMove3DManipulator
  {
  public:
    PlaneManipulator(MovableObject& object, UndoStack& undoStack);
    PlaneManipulator(const PlaneManipulator&) = delete;
    PlaneManipulator& operator = (const PlaneManipulator&) = delete;
    virtual ~PlaneManipulator() noexcept = default;

    virtual std::optional<glm::vec3> tryActivate(
                                  const TouchInfo& touchInfo) noexcept override;
    virtual void release() noexcept override;

  protected:
    virtual void processShift(const glm::vec3& startTouchPoint,
                              const glm::vec3& shiftedPoint) noexcept override;

  private:
    UndoStack& _undoStack;
    MovableObject& _object;

    UndoStack::GroupLocker _groupCommandLocker;

    glm::mat4 _startWorldToObject;
    glm::vec3 _startPosition;
  };
}