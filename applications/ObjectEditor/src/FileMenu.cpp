#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <mtt/editorLib/EditorApplication.h>

#include <AsyncTasks/LoadModelTask.h>
#include <AsyncTasks/SaveModelTask.h>
#include <FileMenu.h>
#include <ObjectEditorCommonData.h>

FileMenu::FileMenu(QWidget& window, ObjectEditorCommonData& commonData) :
  QMenu(tr("&File")),
  _window(window),
  _commonData(commonData)
{
  addAction(tr("&New"), this, &FileMenu::_clearScene);

  QAction* openAction = addAction(tr("&Open"), this, &FileMenu::_loadModel);
  openAction->setShortcut(Qt::CTRL + Qt::Key_O);

  addSeparator();

  QAction* saveAction = addAction(tr("&Save"), this, &FileMenu::_saveModel);
  saveAction->setShortcut(Qt::CTRL + Qt::Key_S);

  addAction(tr("Save &As ..."), this, &FileMenu::_saveModelAs);
}

void FileMenu::_clearScene() noexcept
{
  if (_commonData.scene() == nullptr)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Scene is empty."));
    return;
  }

  _commonData.undoStack().clear();
  _commonData.setDataFilename("");
  _commonData.scene()->dataRoot().clear();
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
