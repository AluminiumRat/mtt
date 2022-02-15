#pragma once

#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Manipulator/UniformScale3DManipulator.h>

namespace mtt
{
  class ScalableObject;
}

class TriangleManipulator : public mtt::UniformScale3DManipulator
{
public:
  TriangleManipulator(mtt::ScalableObject& object, mtt::UndoStack& undoStack);
  TriangleManipulator(const TriangleManipulator&) = delete;
  TriangleManipulator& operator = (const TriangleManipulator&) = delete;
  virtual ~TriangleManipulator() noexcept = default;

  virtual std::optional<glm::vec3> tryActivate(
                                  const TouchInfo& touchInfo) noexcept override;
  virtual void release() noexcept override;

protected:
  virtual void processScale(float scale) noexcept override;

private:
  mtt::UndoStack& _undoStack;
  mtt::ScalableObject& _object;

  mtt::UndoStack::GroupLocker _groupCommandLocker;

  glm::vec3 _startScale;
};