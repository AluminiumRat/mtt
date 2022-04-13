#include <stdexcept>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/application/EditCommands/RemoveObjectCommand.h>
#include <mtt/editorLib/AsyncTasks/AddAnimationFromFbxTask.h>
#include <mtt/editorLib/Objects/SkeletonGroup.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>
#include <mtt/editorLib/EditorApplication.h>

#include <AsyncTasks/AddModelFromFbxTask.h>
#include <Objects/LODObject.h>
#include <Objects/MaterialObject.h>
#include <EditMenu.h>
#include <ObjectEditorCommonData.h>

EditMenu::EditMenu(QWidget& window, ObjectEditorCommonData& commonData) :
  mtt::EditMenu(window, commonData),
  _commonData(commonData)
{
  addSeparator();

  addAction(tr("Add bone"), this, &EditMenu::_addBone);
  addAction(tr("Add LOD"), this, &EditMenu::_addLOD);
  addAction(tr("Add material"), this, &EditMenu::_addMaterial);

  addAction(tr("Add model from Blender's fbx"),
            this,
            &EditMenu::_addModelFromBlender);

  addAction(tr("Add model from 3DMax's fbx"),
            this,
            &EditMenu::_addModelFrom3DMax);

  addAction(tr("Add model from obj"), this, &EditMenu::_addModelFromObj);

  addSeparator();

  addAction(tr("Add animation from fbx"),
            this,
            &EditMenu::_addAnimationFromFbx);
}

void EditMenu::_addBone() noexcept
{
  try
  {
    ObjectEditorScene* scene = _commonData.scene();
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
    if(target == nullptr) target = &scene->dataRoot().skeletonGroup();
    
    std::unique_ptr<mtt::AddObjectCommand> command(
                        new mtt::AddObjectCommand(std::move(newBone), *target));
    _commonData.undoStack().addAndMake(std::move(command));
    _commonData.selectObjects({bonePtr});
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&window(),
                          tr("Unable to add a bone"),
                          error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&window(),
                          tr("Unable to add a bone"),
                          tr("Unknown error"));
  }
}

void EditMenu::_addLOD() noexcept
{
  try
  {
    ObjectEditorScene* scene = _commonData.scene();
    if(scene == nullptr) return;

    std::unique_ptr<LODObject> newLOD(new LODObject(tr("LOD"), true));
    LODObject* lodPtr = newLOD.get();

    std::unique_ptr<mtt::AddObjectCommand> command(
                            new mtt::AddObjectCommand(
                                            std::move(newLOD),
                                            scene->dataRoot().geometryGroup()));
    _commonData.undoStack().addAndMake(std::move(command));
    _commonData.selectObjects({ lodPtr });
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&window(),
                          tr("Unable to add a LOD object"),
                          error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&window(),
                          tr("Unable to add a LOD object"),
                          tr("Unknown error"));
  }
}

void EditMenu::_addMaterial() noexcept
{
  try
  {
    ObjectEditorScene* scene = _commonData.scene();
    if(scene == nullptr) return;

    std::unique_ptr<MaterialObject> newObject(
                                      new MaterialObject(tr("Material"), true));
    MaterialObject* objectPtr = newObject.get();

    std::unique_ptr<mtt::AddObjectCommand> command(
                            new mtt::AddObjectCommand(
                                          std::move(newObject),
                                          scene->dataRoot().materialsGroup()));
    _commonData.undoStack().addAndMake(std::move(command));
    _commonData.selectObjects({ objectPtr });
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&window(),
                          tr("Unable to add material"),
                          error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&window(),
                          tr("Unable to add material"),
                          tr("Unknown error"));
  }
}

void EditMenu::_addModelFromBlender() noexcept
{
  try
  {
    ObjectEditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    QString fileName = QFileDialog::getOpenFileName(&window(),
                                                    tr("Import fbx"),
                                                    "",
                                                    tr("fbx (*.fbx)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<AddModelFromFbxTask> task;
    task.reset(new AddModelFromFbxTask(
                                  fileName,
                                  mtt::BaseFbxImporter::blenderMaterialOptions,
                                  _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&window(),
                          tr("Unable to import fbx"),
                          error.what());
  }
  catch (...)
  {
    QMessageBox::critical(&window(), tr("Error"), tr("Unable to import fbx"));
  }
}

void EditMenu::_addModelFrom3DMax() noexcept
{
  try
  {
    ObjectEditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    QString fileName = QFileDialog::getOpenFileName(&window(),
                                                    tr("Import fbx"),
                                                    "",
                                                    tr("fbx (*.fbx)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<AddModelFromFbxTask> task;
    task.reset(new AddModelFromFbxTask(
                                      fileName,
                                      mtt::BaseFbxImporter::maxMaterialOptions,
                                      _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&window(), tr("Error"), tr("Unable to import fbx"));
  }
}

void EditMenu::_addModelFromObj() noexcept
{
  try
  {
    ObjectEditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    QString fileName = QFileDialog::getOpenFileName(&window(),
                                                    tr("Import obj"),
                                                    "",
                                                    tr("obj (*.obj)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<AddModelFromFbxTask> task;
    task.reset(new AddModelFromFbxTask(
                                      fileName,
                                      mtt::BaseFbxImporter::maxMaterialOptions,
                                      _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&window(),
                          tr("Error"),
                          tr("Unable to import obj file"));
  }
}

void EditMenu::_addAnimationFromFbx() noexcept
{
  try
  {
    ObjectEditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    QString fileName = QFileDialog::getOpenFileName(&window(),
                                                    tr("Import fbx"),
                                                    "",
                                                    tr("fbx (*.fbx)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<mtt::AddAnimationFromFbxTask> task(
                              new mtt::AddAnimationFromFbxTask(
                                            fileName,
                                            scene->dataRoot().animationGroup(),
                                            &scene->dataRoot().skeletonGroup(),
                                            _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&window(), tr("Error"), tr("Unable to import fbx"));
  }
}
