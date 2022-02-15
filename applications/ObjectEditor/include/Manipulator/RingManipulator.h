#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Manipulator/RingRotation3DManipulator.h>

namespace mtt
{
  class RotatableObject;
  class UndoStack;
}

class RingManipulator : public mtt::RingRotation3DManipulator
{
public:
  RingManipulator(mtt::RotatableObject& object, mtt::UndoStack& undoStack);
  RingManipulator(const RingManipulator&) = delete;
  RingManipulator& operator = (const RingManipulator&) = delete;
  virtual ~RingManipulator() noexcept = default;

  virtual std::optional<glm::vec3> tryActivate(
                                  const TouchInfo& touchInfo) noexcept override;
  virtual void release() noexcept override;

protected:
  virtual void processRotation(const glm::mat4& rotation) noexcept override;

private:
  mtt::UndoStack& _undoStack;
  mtt::RotatableObject& _object;

  mtt::UndoStack::GroupLocker _groupCommandLocker;

  glm::mat4 _startObjectToWorld;
  glm::mat4 _startWorldToObject;
  glm::mat4 _startRotation;
};