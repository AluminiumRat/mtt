#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/editorLib/AsyncTasks/LoadEnvironmentTask.h>
#include <mtt/editorLib/AsyncTasks/SaveEnvironmentTask.h>
#include <mtt/editorLib/MainWindow/EnvironmentMenu.h>
#include <mtt/editorLib/Objects/AmbientLightObject.h>
#include <mtt/editorLib/Objects/DirectLightObject.h>
#include <mtt/editorLib/Objects/EnvironmentModel.h>
#include <mtt/editorLib/Objects/SpotLightObject.h>
#include <mtt/editorLib/Objects/PointLightObject.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/editorLib/EditorCommonData.h>

using namespace mtt;

EnvironmentMenu::EnvironmentMenu( QWidget& window,
                                  EditorCommonData& commonData) :
  QMenu(tr("En&vironment")),
  _window(window),
  _commonData(commonData)
{
  addAction(tr("&Load"), this, &EnvironmentMenu::_loadEnvironment);

  addSeparator();

  addAction(tr("&Save"), this, &EnvironmentMenu::_saveEnvironment);
  addAction(tr("Save &As .."), this, &EnvironmentMenu::_saveEnvironmentAs);

  addSeparator();

  addAction(tr("Add ambient light"), this, &EnvironmentMenu::_addAmbientLight);
  addAction(tr("Add direct light"), this, &EnvironmentMenu::_addDirectLight);
  addAction(tr("Add spot light"), this, &EnvironmentMenu::_addSpotLight);
  addAction(tr("Add point light"), this, &EnvironmentMenu::_addPointLight);
  addAction(tr("Add model"), this, &EnvironmentMenu::_addEnvironmentModel);
}

void EnvironmentMenu::_saveEnvironment() noexcept
{
  if(_commonData.environmentFilename().isEmpty()) _saveEnvironmentAs();
  else _saveEnvironmentToFile(_commonData.environmentFilename());
}

void EnvironmentMenu::_saveEnvironmentAs() noexcept
{
  try
  {
    QString fileName = QFileDialog::getSaveFileName(&_window,
                                                    tr("Save environment"),
                                                    "",
                                                    tr("enm (*.enm)"));
    if(fileName.isEmpty()) return;
    _saveEnvironmentToFile(fileName);
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to save model"));
  }
}

void EnvironmentMenu::_saveEnvironmentToFile(const QString& file) noexcept
{
  if (_commonData.scene() == nullptr)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Scene is empty."));
    return;
  }

  try
  {
    std::unique_ptr<mtt::SaveEnvironmentTask> task;
    task.reset(new mtt::SaveEnvironmentTask(*_commonData.scene(),
                                            file,
                                            _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window,
                          tr("Error"),
                          tr("Unable to save environment"));
  }
}

void EnvironmentMenu::_loadEnvironment() noexcept
{
  if (_commonData.scene() == nullptr)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Scene is empty."));
    return;
  }

  try
  {
    QString fileName = QFileDialog::getOpenFileName(&_window,
                                                    tr("Load environment"),
                                                    "",
                                                    tr("enm (*.enm)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<mtt::LoadEnvironmentTask> task;
    task.reset(new mtt::LoadEnvironmentTask(*_commonData.scene(),
                                            fileName,
                                            _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window,
                          tr("Error"),
                          tr("Unable to load environment"));
  }
}

template<typename LightType>
void EnvironmentMenu::_addLight(const QString& name) noexcept
{
  try
  {
    EditorScene* scene = _commonData.scene();
    if(scene == nullptr) return;

    std::unique_ptr<LightType> newLight(new LightType(name, true));
    LightType* lightPtr = newLight.get();

    std::unique_ptr<mtt::AddObjectCommand> command(
            new mtt::AddObjectCommand(std::move(newLight),
                                      scene->environmentRoot().objectsGroup()));
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

void EnvironmentMenu::_addAmbientLight() noexcept
{
  _addLight<mtt::AmbientLightObject>(tr("Ambient light"));
}

void EnvironmentMenu::_addDirectLight() noexcept
{
  _addLight<mtt::DirectLightObject>(tr("Direct light"));
}

void EnvironmentMenu::_addSpotLight() noexcept
{
  _addLight<mtt::SpotLightObject>(tr("Spot light"));
}

void EnvironmentMenu::_addPointLight() noexcept
{
  _addLight<mtt::PointLightObject>(tr("Point light"));
}

void EnvironmentMenu::_addEnvironmentModel() noexcept
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
                                      scene->environmentRoot().objectsGroup()));
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
