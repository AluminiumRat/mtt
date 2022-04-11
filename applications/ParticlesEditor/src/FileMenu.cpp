#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <mtt/editorLib/AsyncTasks/LoadEnvironmentTask.h>
#include <mtt/editorLib/EditorApplication.h>

#include <AsyncTasks/LoadEffectTask.h>
#include <AsyncTasks/SaveEffectTask.h>
#include <FileMenu.h>
#include <MainWindow.h>
#include <ParticlesEditorCommonData.h>

#include <GeneratedFiles/ui_MainWindow.h>

FileMenu::FileMenu( MainWindow& window,
                    Ui_MainWindow& ui,
                    ParticlesEditorCommonData& commonData) :
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
          &FileMenu::_saveEffect,
          Qt::DirectConnection);

  connect(_ui.actionSave_as,
          &QAction::triggered,
          this,
          &FileMenu::_saveEffectAs,
          Qt::DirectConnection);

  connect(_ui.actionLoad,
          &QAction::triggered,
          this,
          &FileMenu::_load,
          Qt::DirectConnection);

  connect(_ui.actionLoad_environment,
          &QAction::triggered,
          this,
          &FileMenu::_loadEnvironment,
          Qt::DirectConnection);
}

void FileMenu::_saveEffect() noexcept
{
  if(_commonData.dataFilename().isEmpty()) _saveEffectAs();
  else _saveToFile(_commonData.dataFilename());
}

void FileMenu::_saveEffectAs() noexcept
{
  try
  {
    QString fileName = QFileDialog::getSaveFileName(&_window,
                                                    tr("Save effect"),
                                                    "",
                                                    tr("pee (*.pee)"));
    if(fileName.isEmpty()) return;
    _saveToFile(fileName);
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to save effect"));
  }
}

void FileMenu::_saveToFile(const QString& file) noexcept
{
  if (_commonData.scene() == nullptr)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Scene is empty."));
    return;
  }

  try
  {
    std::unique_ptr<SaveEffectTask> task;
    task.reset(new SaveEffectTask(*_commonData.scene(),
                                  file,
                                  _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to save model"));
  }
}

void FileMenu::_load() noexcept
{
  if (_commonData.scene() == nullptr)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Scene is empty."));
    return;
  }

  try
  {
    QString fileName = QFileDialog::getOpenFileName(&_window,
                                                    tr("Load effect"),
                                                    "",
                                                    tr("pee (*.pee)"));
    if(fileName.isEmpty()) return;

    std::unique_ptr<LoadEffectTask> task;
    task.reset(new LoadEffectTask(*_commonData.scene(),
                                  fileName,
                                  _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to load effect"));
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
