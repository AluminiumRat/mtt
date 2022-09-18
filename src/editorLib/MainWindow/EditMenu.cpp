#include <stdexcept>

#include <QtWidgets/QMessageBox>

#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/application/EditCommands/RemoveObjectCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/editorLib/MainWindow/EditMenu.h>
#include <mtt/editorLib/EditorCommonData.h>

using namespace mtt;

EditMenu::EditMenu( QWidget& window,
                    EditorCommonData& commonData,
                    std::unique_ptr<CopyToClipboardOperation> copyOperation) :
  QMenu(tr("&Edit")),
  _window(window),
  _commonData(commonData),
  _copyOperation(std::move(copyOperation)),
  _copyAction(nullptr),
  _pasteAction(nullptr),
  _deleteAction(nullptr)
{
  QAction* undoAction = addAction(tr("&Undo"), this, &EditMenu::_undo);
  undoAction->setShortcut(Qt::CTRL + Qt::Key_Z);

  QAction* redoAction = addAction(tr("&Redo"), this, &EditMenu::_redo);
  redoAction->setShortcut(Qt::CTRL + Qt::Key_Y);

  addSeparator();
  if (_copyOperation != nullptr)
  {
    _copyAction = addAction(tr("&Copy"), this, &EditMenu::_copyObject);
    _copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
  }

  _deleteAction = addAction(tr("&Delete"), this, &EditMenu::_deleteObject);
  _deleteAction->setShortcut(Qt::Key_Delete);

  connect(&_commonData,
          &EditorCommonData::selectedObjectsChanged,
          this,
          &EditMenu::_updateCopyDeleteActions,
          Qt::DirectConnection);
  _updateCopyDeleteActions();
}

void EditMenu::_undo() noexcept
{
  try
  {
    _commonData.undoStack().undoCommand();
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window, tr("Error"), error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unknown error"));
  }
}

void EditMenu::_redo() noexcept
{
  try
  {
    _commonData.undoStack().redoCommand();
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window, tr("Error"), error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unknown error"));
  }
}

void EditMenu::_updateCopyDeleteActions() noexcept
{
  bool deleteEnabled = true;
  bool copyEnabled = true;

  if(_commonData.selectedObjects().size() == 0)
  {
    deleteEnabled = false;
    copyEnabled = false;
  }
  else
  {
    for(mtt::Object* object : _commonData.selectedObjects())
    {
      if( object->parent() == nullptr ||
          !object->parent()->subobjectCanBeAddedAndRemoved(*object))
      {
        deleteEnabled = false;
        break;
      }
    }
  }

  if(_deleteAction != nullptr) _deleteAction->setEnabled(deleteEnabled);
  if(_copyAction != nullptr) _copyAction->setEnabled(copyEnabled);
}

void EditMenu::_copyObject() noexcept
{
  try
  {
    _copyOperation->copyToClipboard(_commonData.selectedObjects());
  }
  catch (std::exception& error)
  {
    QMessageBox::critical(&_window, tr("Error"), error.what());
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unknown error"));
  }
}

void EditMenu::_pasteObject() noexcept
{
  try
  {
  }
  catch (std::exception& error)
  {
    QMessageBox::critical(&_window, tr("Error"), error.what());
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unknown error"));
  }
}

void EditMenu::_deleteObject() noexcept
{
  try
  {
    if(_commonData.selectedObjects().empty()) return;

    std::unique_ptr<mtt::CompositeCommand> compositeCommand(
                                                    new mtt::CompositeCommand);

    for (mtt::Object* object : _commonData.selectedObjects())
    {
      if(object->parent() == nullptr) return;
      mtt::Object& parent = *object->parent();

      if(!parent.subobjectCanBeAddedAndRemoved(*object)) return;
      std::unique_ptr<mtt::RemoveObjectCommand> removeCommand(
                                new mtt::RemoveObjectCommand(*object, parent));
      compositeCommand->addSubcommand(std::move(removeCommand));
    }

    _commonData.undoStack().addAndMake(std::move(compositeCommand));
  }
  catch (std::exception& error)
  {
    QMessageBox::critical(&_window, tr("Error"), error.what());
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unknown error"));
  }
}
