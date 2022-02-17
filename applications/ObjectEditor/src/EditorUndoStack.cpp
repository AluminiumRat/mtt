#include <mtt/editorLib/AnimationPlayer.h>

#include <EditorUndoStack.h>

#define STACK_SIZE 20

EditorUndoStack::EditorUndoStack(mtt::AnimationPlayer& animationPlayer) :
  UndoStack(STACK_SIZE),
  _animationPlayer(animationPlayer)
{
}

void EditorUndoStack::addAndMake(
                              std::unique_ptr<mtt::AbstractEditCommand> command)
{
  _animationPlayer.stop();
  UndoStack::addAndMake(std::move(command));
}

void EditorUndoStack::undoCommand()
{
  _animationPlayer.stop();
  UndoStack::undoCommand();
}

void EditorUndoStack::redoCommand()
{
  _animationPlayer.stop();
  UndoStack::redoCommand();
}

void EditorUndoStack::clear() noexcept
{
  _animationPlayer.stop();
  UndoStack::clear();
}
