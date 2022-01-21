#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Manipulator/AxisMove3DManipulator.h>

namespace mtt
{
  class UndoStack;
}

class MovableObject;

class AxisManipulator : public mtt::AxisMove3DManipulator
{
public:
  AxisManipulator(MovableObject& object, mtt::UndoStack& undoStack);
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
  mtt::UndoStack& _undoStack;
  MovableObject& _object;

  mtt::UndoStack::GroupLocker _groupCommandLocker;

  glm::mat4 _startWorldToObject;
  glm::vec3 _startPosition;
};