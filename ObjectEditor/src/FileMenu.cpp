#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <AsyncTasks/LoadModelTask.h>
#include <AsyncTasks/SaveModelTask.h>
#include <EditorApplication.h>
#include <EditorCommonData.h>
#include <FileMenu.h>
#include <MainWindow.h>

#include <GeneratedFiles/ui_MainWindow.h>

FileMenu::FileMenu( MainWindow& window,
                    Ui_MainWindow& ui,
                    EditorCommonData& commonData) :
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
}

void FileMenu::_saveModel() noexcept
{
  if(_commonData.modelFilename().isEmpty()) _saveModelAs();
  else _save(_commonData.modelFilename());
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
    _save(fileName);
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to save model"));
  }
}

void FileMenu::_save(const QString& file) noexcept
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
    EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
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
    EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to load model"));
  }
}
