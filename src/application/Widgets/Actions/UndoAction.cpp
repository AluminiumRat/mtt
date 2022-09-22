#include <stdexcept>

#include <QtWidgets/QMessageBox>

#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Widgets/Actions/UndoAction.h>

using namespace mtt;

UndoAction::UndoAction(UndoStack& undoStack, QWidget* dialogParentWidget) :
  QAction(tr("&Undo")),
  _undoStack(undoStack),
  _dialogParentWidget(dialogParentWidget)
{
  setShortcut(Qt::CTRL + Qt::Key_Z);

  connect(&_undoStack,
          &UndoStack::undoAvailableChanged,
          this,
          &UndoAction::setEnabled,
          Qt::DirectConnection);
  setEnabled(_undoStack.undoAvailable());

  connect(this,
          &UndoAction::triggered,
          this,
          &UndoAction::_makeUndo,
          Qt::DirectConnection);
}

void UndoAction::_makeUndo() noexcept
{
  try
  {
    _undoStack.undoCommand();
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(_dialogParentWidget,
                          tr("Error"),
                          error.what());
  }
  catch(...)
  {
    QMessageBox::critical(_dialogParentWidget,
                          tr("Error"),
                          tr("Unknown error"));
  }
}
