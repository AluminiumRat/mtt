#include <stdexcept>

#include <QtWidgets/QMessageBox>

#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/application/EditCommands/RemoveObjectCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Widgets/Actions/DeleteAction.h>
#include <mtt/application/CommonEditData.h>

using namespace mtt;

DeleteAction::DeleteAction( CommonEditData& commonData,
                            QWidget* dialogParentWidget) :
  QAction(tr("&Delete")),
  _commonData(commonData),
  _dialogParentWidget(dialogParentWidget)
{
  setShortcut(Qt::Key_Delete);

  connect(&_commonData,
          &CommonEditData::selectedObjectsChanged,
          this,
          &DeleteAction::_updatEnabled,
          Qt::DirectConnection);
  _updatEnabled();

  connect(this,
          &DeleteAction::triggered,
          this,
          &DeleteAction::_makeDelete,
          Qt::DirectConnection);
}

void DeleteAction::_updatEnabled() noexcept
{
  bool enabledValue = true;

  if (_commonData.selectedObjects().size() == 0) enabledValue = false;
  else
  {
    for (Object* object : _commonData.selectedObjects())
    {
      if (object->parent() == nullptr ||
          !object->parent()->subobjectCanBeAddedAndRemoved(*object))
      {
        enabledValue = false;
      }
    }
  }

  setEnabled(enabledValue);
}

void DeleteAction::_makeDelete() noexcept
{
  try
  {
    if(_commonData.selectedObjects().empty()) return;

    std::unique_ptr<CompositeCommand> compositeCommand(new CompositeCommand);

    for (Object* object : _commonData.selectedObjects())
    {
      if(object->parent() == nullptr) return;
      Object& parent = *object->parent();

      if(!parent.subobjectCanBeAddedAndRemoved(*object)) return;
      std::unique_ptr<RemoveObjectCommand> removeCommand(
                                      new RemoveObjectCommand(*object, parent));
      compositeCommand->addSubcommand(std::move(removeCommand));
    }

    _commonData.undoStack().addAndMake(std::move(compositeCommand));
  }
  catch (std::exception& error)
  {
    QMessageBox::critical(_dialogParentWidget, tr("Error"), error.what());
  }
  catch (...)
  {
    QMessageBox::critical(_dialogParentWidget,
                          tr("Error"),
                          tr("Unknown error"));
  }
}
