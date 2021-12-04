#include <stdexcept>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <mtt/Application/EditCommands/AddObjectCommand.h>
#include <mtt/Application/EditCommands/CompositeCommand.h>
#include <mtt/Application/EditCommands/RemoveObjectCommand.h>

#include <AsyncTasks/AddAnimationFromFbxTask.h>
#include <AsyncTasks/AddModelFromFbxTask.h>
#include <Objects/AmbientLightObject.h>
#include <Objects/DirectLightObject.h>
#include <Objects/LODObject.h>
#include <Objects/SkeletonGroup.h>
#include <Objects/SkeletonObject.h>
#include <EditMenu.h>
#include <EditorApplication.h>
#include <MainWindow.h>

#include <GeneratedFiles/ui_MainWindow.h>

EditMenu::EditMenu( MainWindow& window,
                    Ui_MainWindow& ui,
                    EditorCommonData& commonData) :
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
          &EditorCommonData::selectedObjectsChanged,
          this,
          &EditMenu::_updateDeleteAction,
          Qt::DirectConnection);
  _updateDeleteAction();

  connect(_ui.actionDelete,
          &QAction::triggered,
          this,
          &EditMenu::_deleteObject,
          Qt::DirectConnection);

  connect(_ui.actionAdd_bone,
          &QAction::triggered,
          this,
          &EditMenu::_addBone,
          Qt::DirectConnection);

  connect(_ui.actionAdd_LOD,
          &QAction::triggered,
          this,
          &EditMenu::_addLOD,
          Qt::DirectConnection);

  connect(_ui.actionAdd_model_from_Blender,
          &QAction::triggered,
          this,
          &EditMenu::_addModelFromBlender,
          Qt::DirectConnection);

  connect(_ui.actionAdd_model_from_3DMax,
          &QAction::triggered,
          this,
          &EditMenu::_addModelFrom3DMax,
          Qt::DirectConnection);

  connect(_ui.actionAdd_model_from_obj,
          &QAction::triggered,
          this,
          &EditMenu::_addModelFromObj,
          Qt::DirectConnection);

  connect(_ui.actionAdd_animation_from_fbx,
          &QAction::triggered,
          this,
          &EditMenu::_addAnimationFromFbx,
          Qt::DirectConnection);

  connect(_ui.actionAdd_ambient_light,
          &QAction::triggered,
          this,
          &EditMenu::_addAmbientLight,
          Qt::DirectConnection);

  connect(_ui.actionAdd_direct_light,
          &QAction::triggered,
          this,
          &EditMenu::_addDirectLight,
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

void EditMenu::_addBone() noexcept
{
  try
  {
    EditorScene* scene = _commonData.scene();
    if(scene == nullptr) return;

    std::unique_ptr<SkeletonObject> newBone(
                                          new SkeletonObject(tr("bone"), true));
    SkeletonObject* bonePtr = newBone.get();

    mtt::Object* target = nullptr;    
    if (_commonData.selectedObjects().size() == 1)
    {
      target = _commonData.selectedObjects()[0];
      if(!target->subobjectCanBeAddedAndRemoved(*newBone)) target = nullptr;
    }
    if(target == nullptr) target = &scene->root().skeletonGroup();
    
    std::unique_ptr<mtt::AddObjectCommand> command(
                        new mtt::AddObjectCommand(std::move(newBone), *target));
    _commonData.undoStack().addAndMake(std::move(command));
    _commonData.selectObjects({bonePtr});
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add a bone"),
                          error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add a bone"),
                          tr("Unknown error"));
  }
}

void EditMenu::_addLOD() noexcept
{
  try
  {
    EditorScene* scene = _commonData.scene();
    if(scene == nullptr) return;

    std::unique_ptr<LODObject> newLOD(new LODObject(tr("LOD"), true));
    LODObject* lodPtr = newLOD.get();

    std::unique_ptr<mtt::AddObjectCommand> command(
                                new mtt::AddObjectCommand(
                                                std::move(newLOD),
                                                scene->root().geometryGroup()));
    _commonData.undoStack().addAndMake(std::move(command));
    _commonData.selectObjects({ lodPtr });
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add a LOD object"),
                          error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add a LOD object"),
                          tr("Unknown error"));
  }
}

void EditMenu::_addModelFromBlender() noexcept
{
  try
  {
    EditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    QString fileName = QFileDialog::getOpenFileName(&_window,
                                                    tr("Import fbx"),
                                                    "",
                                                    tr("fbx (*.fbx)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<AddModelFromFbxTask> task;
    task.reset(new AddModelFromFbxTask(
                                  fileName,
                                  mtt::BaseFbxImporter::blenderMaterialOptions,
                                  _commonData));
    EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to import fbx"));
  }
}

void EditMenu::_addModelFrom3DMax() noexcept
{
  try
  {
    EditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    QString fileName = QFileDialog::getOpenFileName(&_window,
                                                    tr("Import fbx"),
                                                    "",
                                                    tr("fbx (*.fbx)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<AddModelFromFbxTask> task;
    task.reset(new AddModelFromFbxTask(
                                      fileName,
                                      mtt::BaseFbxImporter::maxMaterialOptions,
                                      _commonData));
    EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to import fbx"));
  }
}

void EditMenu::_addModelFromObj() noexcept
{
  try
  {
    EditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    QString fileName = QFileDialog::getOpenFileName(&_window,
                                                    tr("Import obj"),
                                                    "",
                                                    tr("obj (*.obj)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<AddModelFromFbxTask> task;
    task.reset(new AddModelFromFbxTask(
                                      fileName,
                                      mtt::BaseFbxImporter::maxMaterialOptions,
                                      _commonData));
    EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window,
                          tr("Error"),
                          tr("Unable to import obj file"));
  }
}

void EditMenu::_addAnimationFromFbx() noexcept
{
  try
  {
    EditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    QString fileName = QFileDialog::getOpenFileName(&_window,
                                                    tr("Import fbx"),
                                                    "",
                                                    tr("fbx (*.fbx)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<AddAnimationFromFbxTask> task(
                            new AddAnimationFromFbxTask(fileName, _commonData));
    EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to import fbx"));
  }
}

void EditMenu::_addAmbientLight() noexcept
{
  try
  {
    EditorScene* scene = _commonData.scene();
    if(scene == nullptr) return;

    std::unique_ptr<AmbientLightObject> newLight(
                            new AmbientLightObject(tr("Ambient light"), true));
    AmbientLightObject* lightPtr = newLight.get();

    std::unique_ptr<mtt::AddObjectCommand> command(
                        new mtt::AddObjectCommand(std::move(newLight),
                                                  scene->root().environment()));
    _commonData.undoStack().addAndMake(std::move(command));
    _commonData.selectObjects({lightPtr});
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add a light"),
                          error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add a light"),
                          tr("Unknown error"));
  }
}

void EditMenu::_addDirectLight() noexcept
{
  try
  {
    EditorScene* scene = _commonData.scene();
    if(scene == nullptr) return;

    std::unique_ptr<DirectLightObject> newLight(
                              new DirectLightObject(tr("Direct light"), true));
    DirectLightObject* lightPtr = newLight.get();

    std::unique_ptr<mtt::AddObjectCommand> command(
                        new mtt::AddObjectCommand(std::move(newLight),
                                                  scene->root().environment()));
    _commonData.undoStack().addAndMake(std::move(command));
    _commonData.selectObjects({lightPtr});
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add a light"),
                          error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add a light"),
                          tr("Unknown error"));
  }
}
