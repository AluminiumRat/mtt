#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Manipulator/AxisMove3DManipulator.h>

namespace mtt
{
  class MovableObject;
  class UndoStack;

  class AxisManipulator : public AxisMove3DManipulator
  {
  public:
    AxisManipulator(MovableObject& object, UndoStack& undoStack);
    AxisManipulator(const AxisManipulator&) = delete;
    AxisManipulator& operator = (const AxisManipulator&) = delete;
    virtual ~AxisManipulator() noexcept = default;

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
