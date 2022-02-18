#pragma once

#include <mtt/application/EditCommands/UndoStack.h>

namespace mtt
{
  class AnimationPlayer;

  class EditorUndoStack : public UndoStack
  {
  public:
    explicit EditorUndoStack(AnimationPlayer& animationPlayer);
    EditorUndoStack(const EditorUndoStack&) = delete;
    EditorUndoStack& operator = (const EditorUndoStack&) = delete;
    virtual ~EditorUndoStack() noexcept = default;

    virtual void addAndMake(
                        std::unique_ptr<AbstractEditCommand> command) override;
    virtual void undoCommand() override;
    virtual void redoCommand() override;
    virtual void clear() noexcept override;

  private:
    AnimationPlayer& _animationPlayer;
  };
}
