#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <mtt/editorLib/EditorApplication.h>

#include <AsyncTasks/EffectExportTask.h>
#include <AsyncTasks/LoadEffectTask.h>
#include <AsyncTasks/SaveEffectTask.h>
#include <ExportOptionsDialog.h>
#include <FileMenu.h>
#include <ParticlesEditorCommonData.h>

FileMenu::FileMenu( QWidget& window, ParticlesEditorCommonData& commonData) :
  QMenu(tr("&File")),
  _window(window),
  _commonData(commonData)
{
  addAction(tr("&New"), this, &FileMenu::_clearScene);
  addSeparator();

  QAction* openAction = addAction(tr("&Open"), this, &FileMenu::_load);
  openAction->setShortcut(Qt::CTRL + Qt::Key_O);

  addSeparator();

  QAction* saveAction = addAction(tr("&Save"), this, &FileMenu::_saveEffect);
  saveAction->setShortcut(Qt::CTRL + Qt::Key_S);

  addAction(tr("Save &As ..."), this, &FileMenu::_saveEffectAs);

  addSeparator();
  addAction(tr("&Export"), this, &FileMenu::_export);
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
  _commonData.scene()->changeDataRoot(
                            std::make_unique<RootObject>(tr("Effect"), false));
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
    task.reset(new SaveEffectTask(_commonData.scene()->dataRoot(),
                                  file,
                                  _commonData));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to save effect"));
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

void FileMenu::_export() noexcept
{
  if (_commonData.scene() == nullptr)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Scene is empty."));
    return;
  }

  try
  {
    QString fileName = QFileDialog::getSaveFileName(&_window,
                                                    tr("Export"),
                                                    "",
                                                    tr("pst (*.pst)"));
    if(fileName.isEmpty()) return;

    ExportOptionsDialog optionsDialog(&_window);
    if(optionsDialog.exec() == QDialog::Rejected) return;
    EffectExportTask::ExportOptions options = optionsDialog.getOptions();

    std::unique_ptr<EffectExportTask> task;
    task.reset(new EffectExportTask(*_commonData.scene(),
                                    fileName,
                                    options));
    mtt::EditorApplication::instance().asyncTaskQueue.addTask(std::move(task));
  }
  catch (...)
  {
    QMessageBox::critical(&_window, tr("Error"), tr("Unable to export effect"));
  }
}
