#include <stdexcept>

#include <QtWidgets/QMessageBox>

#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Widgets/Actions/RedoAction.h>

using namespace mtt;

RedoAction::RedoAction(UndoStack& undoStack, QWidget* dialogParentWidget) :
  QAction(tr("&Redo")),
  _undoStack(undoStack),
  _dialogParentWidget(dialogParentWidget)
{
  setShortcut(Qt::CTRL + Qt::Key_Y);

  connect(&_undoStack,
          &UndoStack::redoAvailableChanged,
          this,
          &RedoAction::setEnabled,
          Qt::DirectConnection);
  setEnabled(_undoStack.redoAvailable());

  connect(this,
          &RedoAction::triggered,
          this,
          &RedoAction::_makeRedo,
          Qt::DirectConnection);
}

void RedoAction::_makeRedo() noexcept
{
  try
  {
    _undoStack.redoCommand();
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
