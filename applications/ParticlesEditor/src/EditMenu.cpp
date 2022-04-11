#include <stdexcept>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/application/EditCommands/RemoveObjectCommand.h>
#include <mtt/editorLib/EditorApplication.h>

#include <AsyncTasks/ImportAnimationTask.h>
#include <Objects/Fluid/BlockerObject.h>
#include <Objects/Fluid/GasSource.h>
#include <Objects/Fluid/HeaterObject.h>
#include <Objects/EmitterObject.h>
#include <Objects/FrameObject.h>
#include <Objects/GravityModificator.h>
#include <Objects/ParticleAnimation.h>
#include <Objects/SizeControlObject.h>
#include <Objects/VisibilityControlObject.h>
#include <EditMenu.h>
#include <MainWindow.h>

#include <GeneratedFiles/ui_MainWindow.h>

EditMenu::EditMenu( MainWindow& window,
                    Ui_MainWindow& ui,
                    ParticlesEditorCommonData& commonData) :
  _window(window),
  _ui(ui),
  _commonData(commonData)
{
}

void EditMenu::setupUI()
{
  connect(_ui.actionUndo,
          &QAction::triggered,
          this,
          &EditMenu::_undo,
          Qt::DirectConnection);

  connect(_ui.actionRedo,
          &QAction::triggered,
          this,
          &EditMenu::_redo,
          Qt::DirectConnection);

  connect(&_commonData,
          &ParticlesEditorCommonData::selectedObjectsChanged,
          this,
          &EditMenu::_updateDeleteAction,
          Qt::DirectConnection);
  _updateDeleteAction();

  connect(_ui.actionDelete,
          &QAction::triggered,
          this,
          &EditMenu::_deleteObject,
          Qt::DirectConnection);

  connect(_ui.actionAdd_frame,
          &QAction::triggered,
          this,
          &EditMenu::_addFrame,
          Qt::DirectConnection);

  connect(_ui.actionAdd_emitter,
          &QAction::triggered,
          this,
          &EditMenu::_addEmitter,
          Qt::DirectConnection);

  connect(_ui.actionAdd_visibility_control,
          &QAction::triggered,
          this,
          &EditMenu::_addVisibilityControl,
          Qt::DirectConnection);

  connect(_ui.actionAdd_size_control,
          &QAction::triggered,
          this,
          &EditMenu::_addSizeControl,
          Qt::DirectConnection);

  connect(_ui.actionAdd_gravity,
          &QAction::triggered,
          this,
          &EditMenu::_addGravity,
          Qt::DirectConnection);

  connect(_ui.actionAdd_blocker,
          &QAction::triggered,
          this,
          &EditMenu::_addBlocker,
          Qt::DirectConnection);

  connect(_ui.actionAdd_heater,
          &QAction::triggered,
          this,
          &EditMenu::_addHeater,
          Qt::DirectConnection);

  connect(_ui.actionAdd_gas_source,
          &QAction::triggered,
          this,
          &EditMenu::_addGasSource,
          Qt::DirectConnection);

  connect(_ui.actionAdd_animation_from_fbx,
          &QAction::triggered,
          this,
          &EditMenu::_addAnimationFromFbx,
          Qt::DirectConnection);
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

void EditMenu::_updateDeleteAction() noexcept
{
  bool deleteEnabled = true;

  if(_commonData.selectedObjects().size() == 0) deleteEnabled = false;
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

  _ui.actionDelete->setEnabled(deleteEnabled);
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

void EditMenu::_addHierarhical(std::unique_ptr<HierarhicalObject> object)
{
  ParticlesEditorScene* scene = _commonData.scene();
  if (scene == nullptr) return;

  HierarhicalObject* objectPtr = object.get();

  mtt::Object* target = nullptr;
  if (_commonData.selectedObjects().size() == 1)
  {
    target = _commonData.selectedObjects()[0];
    if(!target->subobjectCanBeAddedAndRemoved(*object)) target = nullptr;
  }
  if(target == nullptr) target = &scene->dataRoot().modificatorsGroup();
    
  std::unique_ptr<mtt::AddObjectCommand> command(
                        new mtt::AddObjectCommand(std::move(object), *target));
  _commonData.undoStack().addAndMake(std::move(command));
  _commonData.selectObjects({objectPtr});
}

void EditMenu::_addFrame() noexcept
{
  try
  {
    std::unique_ptr<FrameObject> newFrame(new FrameObject(tr("Frame"), true));
    _addHierarhical(std::move(newFrame));
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add a frame"),
                          error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add a frame"),
                          tr("Unknown error"));
  }
}

template <typename Modificator>
void EditMenu::_addModificator( const QString& name,
                                const QString& errorString) noexcept
{
  try
  {
    ParticlesEditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    std::unique_ptr<Modificator> newObject(new Modificator(name, true));
    newObject->fieldRef().set(&scene->dataRoot().particleField());
    _addHierarhical(std::move(newObject));
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window, errorString, error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&_window, errorString, tr("Unknown error"));
  }
}

void EditMenu::_addEmitter() noexcept
{
  _addModificator<EmitterObject>(tr("Emitter"), tr("Unable to add a emitter"));
}

void EditMenu::_addVisibilityControl() noexcept
{
  _addModificator<VisibilityControlObject>(
                                      tr("Visibility control"),
                                      tr("Unable to add a visibility control"));
}

void EditMenu::_addSizeControl() noexcept
{
  _addModificator<SizeControlObject>( tr("Size control"),
                                      tr("Unable to add a size control"));
}

void EditMenu::_addGravity() noexcept
{
  _addModificator<GravityModificator>(tr("Gravity"),
                                      tr("Unable to add a gravity"));
}

void EditMenu::_addBlocker() noexcept
{
  _addModificator<BlockerObject>( tr("Blocker"),
                                  tr("Unable to add a blocker"));
}

void EditMenu::_addHeater() noexcept
{
  _addModificator<HeaterObject>(tr("Heater"),
                                tr("Unable to add a heater"));
}

void EditMenu::_addGasSource() noexcept
{
  _addModificator<GasSource>( tr("Gas source"),
                              tr("Unable to add a gas source"));
}

void EditMenu::_addAnimationFromFbx() noexcept
{
  try
  {
    ParticlesEditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    QString fileName = QFileDialog::getOpenFileName(&_window,
                                                    tr("Import fbx"),
                                                    "",
                                                    tr("fbx (*.fbx)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<ImportAnimationTask> task(
                                  new ImportAnimationTask(fileName,
                                                          _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to import fbx"));
  }
}
