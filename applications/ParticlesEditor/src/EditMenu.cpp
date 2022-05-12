#include <stdexcept>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/editorLib/EditorApplication.h>

#include <AsyncTasks/ImportAnimationTask.h>
#include <Objects/Fluid/BlockerObject.h>
#include <Objects/Fluid/GasSource.h>
#include <Objects/Fluid/HeaterObject.h>
#include <Objects/EmitterObject.h>
#include <Objects/FrameObject.h>
#include <Objects/GasEmissionAction.h>
#include <Objects/GravityModificator.h>
#include <Objects/HeatingAction.h>
#include <Objects/ParticleAnimation.h>
#include <Objects/ParticlesEmissionAction.h>
#include <Objects/SizeControlObject.h>
#include <Objects/VisibilityControlObject.h>
#include <EditMenu.h>
#include <ParticlesEditorCommonData.h>

EditMenu::EditMenu( QWidget& window,
                    ParticlesEditorCommonData& commonData) :
  mtt::EditMenu(window, commonData),
  _commonData(commonData)
{
  addSeparator();

  addAction(tr("Add frame"), this, &EditMenu::_addFrame);
  addAction(tr("Add emitter"), this, &EditMenu::_addEmitter);
  addAction(tr("Add visibility control"),
            this,
            &EditMenu::_addVisibilityControl);
  addAction(tr("Add size control"), this, &EditMenu::_addSizeControl);
  addAction(tr("Add gravity"), this, &EditMenu::_addGravity);
  addAction(tr("Add blocker"), this, &EditMenu::_addBlocker);
  addAction(tr("Add heater"), this, &EditMenu::_addHeater);
  addAction(tr("Add gas source"), this, &EditMenu::_addGasSource);

  addSeparator();

  addAction(tr("Add animation from fbx"),
            this,
            &EditMenu::_addAnimationFromFbx);

  addAction(tr("Add particles emission action"),
            this,
            &EditMenu::_addParticlesEmissionAction);

  addAction(tr("Add gas emission action"),
            this,
            &EditMenu::_addGasEmissionAction);

  addAction(tr("Add heating action"),
            this,
            &EditMenu::_addHeatingAction);
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
    QMessageBox::critical(&window(),
                          tr("Unable to add a frame"),
                          error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&window(),
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
    QMessageBox::critical(&window(), errorString, error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&window(), errorString, tr("Unknown error"));
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

    QString fileName = QFileDialog::getOpenFileName(&window(),
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
    QMessageBox::critical(&window(), tr("Error"), tr("Unable to import fbx"));
  }
}

template <typename Action>
void EditMenu::_addAction(const QString& name,
                          const QString& errorString) noexcept
{
  try
  {
    ParticlesEditorScene* scene = _commonData.scene();
    if (scene == nullptr) return;

    std::unique_ptr<Action> newObject(new Action(name, true));
    Action* objectPtr = newObject.get();

    std::unique_ptr<mtt::AddObjectCommand> command(
                      new mtt::AddObjectCommand(std::move(newObject),
                                                scene->dataRoot().animation()));
    _commonData.undoStack().addAndMake(std::move(command));
    _commonData.selectObjects({objectPtr});
  }
  catch(std::exception& error)
  {
    QMessageBox::critical(&window(), errorString, error.what());
  }
  catch(...)
  {
    QMessageBox::critical(&window(), errorString, tr("Unknown error"));
  }
}

void EditMenu::_addParticlesEmissionAction() noexcept
{
  _addAction<ParticlesEmissionAction>(
                                tr("Particles emission"),
                                tr("Unable to add particles emission action"));
}

void EditMenu::_addGasEmissionAction() noexcept
{
  _addAction<GasEmissionAction>(tr("Gas emission"),
                            tr("Unable to add gas emission action"));
}

void EditMenu::_addHeatingAction() noexcept
{
  _addAction<HeatingAction>(tr("Heating action"),
                            tr("Unable to add heating action"));
}
