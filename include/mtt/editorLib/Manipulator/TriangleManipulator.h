#pragma once

#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Manipulator/UniformScale3DManipulator.h>

namespace mtt
{
  class ScalableObject;

  class TriangleManipulator : public UniformScale3DManipulator
  {
  public:
    TriangleManipulator(ScalableObject& object, UndoStack& undoStack);
    TriangleManipulator(const TriangleManipulator&) = delete;
    TriangleManipulator& operator = (const TriangleManipulator&) = delete;
    virtual ~TriangleManipulator() noexcept = default;

    virtual std::optional<glm::vec3> tryActivate(
                                  const TouchInfo& touchInfo) noexcept override;
    virtual void release() noexcept override;

  protected:
    virtual void processScale(float scale) noexcept override;

  private:
    UndoStack& _undoStack;
    ScalableObject& _object;

    UndoStack::GroupLocker _groupCommandLocker;

    glm::vec3 _startScale;
  };
}