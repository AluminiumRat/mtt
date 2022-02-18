#include <stdexcept>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/application/EditCommands/RemoveObjectCommand.h>
#include <mtt/editorLib/AsyncTasks/AddAnimationFromFbxTask.h>
#include <mtt/editorLib/Objects/AmbientLightObject.h>
#include <mtt/editorLib/Objects/DirectLightObject.h>
#include <mtt/editorLib/Objects/EnvironmentModel.h>
#include <mtt/editorLib/Objects/SkeletonGroup.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>

#include <AsyncTasks/AddModelFromFbxTask.h>
#include <Objects/LODObject.h>
#include <Objects/MaterialObject.h>
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

  connect(_ui.actionAdd_material,
          &QAction::triggered,
          this,
          &EditMenu::_addMaterial,
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

  connect(_ui.actionAdd_environment_model,
          &QAction::triggered,
          this,
          &EditMenu::_addEnvironmentModel,
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

    std::unique_ptr<mtt::SkeletonObject> newBone(
                                    new mtt::SkeletonObject(tr("bone"), true));
    mtt::SkeletonObject* bonePtr = newBone.get();

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

void EditMenu::_addMaterial() noexcept
{
  try
  {
    EditorScene* scene = _commonData.scene();
    if(scene == nullptr) return;

    std::unique_ptr<MaterialObject> newObject(
                                      new MaterialObject(tr("Material"), true));
    MaterialObject* objectPtr = newObject.get();

    std::unique_ptr<mtt::AddObjectCommand> command(
                                new mtt::AddObjectCommand(
                                              std::move(newObject),
                                              scene->root().materialsGroup()));
    _commonData.undoStack().addAndMake(std::move(command));
    _commonData.selectObjects({ objectPtr });
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add material"),
                          error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add material"),
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
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to import fbx"),
                          error.what());
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

    std::unique_ptr<mtt::AddAnimationFromFbxTask> task(
                                  new mtt::AddAnimationFromFbxTask(
                                                fileName,
                                                scene->root().animationGroup(),
                                                &scene->root().skeletonGroup(),
                                                _commonData));
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

    std::unique_ptr<mtt::AmbientLightObject> newLight(
                        new mtt::AmbientLightObject(tr("Ambient light"), true));
    mtt::AmbientLightObject* lightPtr = newLight.get();

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

    std::unique_ptr<mtt::DirectLightObject> newLight(
                          new mtt::DirectLightObject(tr("Direct light"), true));
    mtt::DirectLightObject* lightPtr = newLight.get();

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

void EditMenu::_addEnvironmentModel() noexcept
{
  try
  {
    EditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    QString fileName = QFileDialog::getOpenFileName(
                                                &_window,
                                                tr("Import model"),
                                                "",
                                                tr("Models (*.mmd *.fbx)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<mtt::EnvironmentModel> newModel(
                                  new mtt::EnvironmentModel(tr("Model"), true));
    newModel->setFilename(fileName);
    mtt::EnvironmentModel* modelPtr = newModel.get();

    std::unique_ptr<mtt::AddObjectCommand> command(
                        new mtt::AddObjectCommand(std::move(newModel),
                                                  scene->root().environment()));
    _commonData.undoStack().addAndMake(std::move(command));
    _commonData.selectObjects({modelPtr});
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&_window,
                          tr("Unable to add model"),
                          error.what());
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to add model"));
  }
}
