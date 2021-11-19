#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/Core/EditCommands/UndoStack.h>
#include <mtt/Core/Manipulator/PlaneMove3DManipulator.h>

class MovableObject;

class PlaneManipulator : public mtt::PlaneMove3DManipulator
{
public:
  PlaneManipulator(MovableObject& object, mtt::UndoStack& undoStack);
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
  mtt::UndoStack& _undoStack;
  MovableObject& _object;

  mtt::UndoStack::GroupLocker _groupCommandLocker;

  glm::mat4 _startWorldToObject;
  glm::vec3 _startPosition;
};