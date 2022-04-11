#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <mtt/editorLib/EditorApplication.h>

#include <AsyncTasks/LoadEnvironmentTask.h>
#include <AsyncTasks/LoadModelTask.h>
#include <AsyncTasks/SaveEnvironmentTask.h>
#include <AsyncTasks/SaveModelTask.h>
#include <FileMenu.h>
#include <MainWindow.h>
#include <ObjectEditorCommonData.h>

#include <GeneratedFiles/ui_MainWindow.h>

FileMenu::FileMenu( MainWindow& window,
                    Ui_MainWindow& ui,
                    ObjectEditorCommonData& commonData) :
  _window(window),
  _ui(ui),
  _commonData(commonData)
{
}

void FileMenu::setupUI()
{
  connect(_ui.actionSave,
          &QAction::triggered,
          this,
          &FileMenu::_saveModel,
          Qt::DirectConnection);

  connect(_ui.actionSave_as,
          &QAction::triggered,
          this,
          &FileMenu::_saveModelAs,
          Qt::DirectConnection);

  connect(_ui.actionLoad,
          &QAction::triggered,
          this,
          &FileMenu::_loadModel,
          Qt::DirectConnection);

  connect(_ui.actionSave_environment,
          &QAction::triggered,
          this,
          &FileMenu::_saveEnvironment,
          Qt::DirectConnection);

  connect(_ui.actionSave_environment_as,
          &QAction::triggered,
          this,
          &FileMenu::_saveEnvironmentAs,
          Qt::DirectConnection);

  connect(_ui.actionLoad_environment,
          &QAction::triggered,
          this,
          &FileMenu::_loadEnvironment,
          Qt::DirectConnection);
}

void FileMenu::_saveModel() noexcept
{
  if(_commonData.dataFilename().isEmpty()) _saveModelAs();
  else _saveModelToFile(_commonData.dataFilename());
}

void FileMenu::_saveModelAs() noexcept
{
  try
  {
    QString fileName = QFileDialog::getSaveFileName(&_window,
                                                    tr("Save model"),
                                                    "",
                                                    tr("mmd (*.mmd)"));
    if(fileName.isEmpty()) return;
    _saveModelToFile(fileName);
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to save model"));
  }
}

void FileMenu::_saveModelToFile(const QString& file) noexcept
{
  if (_commonData.scene() == nullptr)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Scene is empty."));
    return;
  }

  try
  {
    std::unique_ptr<SaveModelTask> task;
    task.reset(new SaveModelTask( *_commonData.scene(),
                                  file,
                                  _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to save model"));
  }
}

void FileMenu::_loadModel() noexcept
{
  if (_commonData.scene() == nullptr)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Scene is empty."));
    return;
  }

  try
  {
    QString fileName = QFileDialog::getOpenFileName(&_window,
                                                    tr("Load model"),
                                                    "",
                                                    tr("mmd (*.mmd)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<LoadModelTask> task;
    task.reset(new LoadModelTask( *_commonData.scene(),
                                  fileName,
                                  _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to load model"));
  }
}

void FileMenu::_saveEnvironment() noexcept
{
  if(_commonData.environmentFilename().isEmpty()) _saveEnvironmentAs();
  else _saveEnvironmentToFile(_commonData.environmentFilename());
}

void FileMenu::_saveEnvironmentAs() noexcept
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

void FileMenu::_saveEnvironmentToFile(const QString& file) noexcept
{
  if (_commonData.scene() == nullptr)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Scene is empty."));
    return;
  }

  try
  {
    std::unique_ptr<SaveEnvironmentTask> task;
    task.reset(new SaveEnvironmentTask( *_commonData.scene(),
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

void FileMenu::_loadEnvironment() noexcept
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

    std::unique_ptr<LoadEnvironmentTask> task;
    task.reset(new LoadEnvironmentTask( *_commonData.scene(),
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
