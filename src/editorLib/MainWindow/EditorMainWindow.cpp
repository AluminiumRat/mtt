#include <QtWidgets/QMessageBox>

#include <mtt/editorLib/MainWindow/EditorMainWindow.h>
#include <mtt/editorLib/EditorApplication.h>

using namespace mtt;

EditorMainWindow::EditorMainWindow() :
  _asyncTaskDialog(mtt::EditorApplication::instance().asyncTaskQueue)
{
  _asyncTaskDialog.setModal(true);

  connect(&mtt::EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::taskStarted,
          this,
          &EditorMainWindow::_showAsyncTaskDialog,
          Qt::DirectConnection);

  connect(&mtt::EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::taskFinished,
          this,
          &EditorMainWindow::_checkAsyncTaskDialog,
          Qt::DirectConnection);

  connect(&mtt::EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::warningEmitted,
          this,
          &EditorMainWindow::_processWarning);

  connect(&mtt::EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::errorEmitted,
          this,
          &EditorMainWindow::_processError);
}

void EditorMainWindow::_showAsyncTaskDialog(mtt::AbstractAsyncTask& task)
{
  if(task.displayBehavior() == mtt::AbstractAsyncTask::EXPLICIT)
  {
    _asyncTaskDialog.show();
  }
}

void EditorMainWindow::_checkAsyncTaskDialog()
{
  mtt::AsyncTaskQueue::Counters counters =
                  mtt::EditorApplication::instance().asyncTaskQueue.counters();
  if(counters.explicitFinishedInChunk == counters.explicitInChunk)
  {
    _asyncTaskDialog.hide();
  }
}

void EditorMainWindow::_processWarning( mtt::AbstractAsyncTask& task,
                                  const QString& message) noexcept
{
  QMessageBox* dialog = new QMessageBox(&_asyncTaskDialog);
  dialog->setWindowTitle(task.name());
  dialog->setText(message);
  dialog->setIcon(QMessageBox::Warning);
  dialog->setModal(true);
  dialog->show();
}

void EditorMainWindow::_processError( mtt::AbstractAsyncTask& task,
                                const std::exception& error) noexcept
{
  QMessageBox* errorDialog = new QMessageBox(&_asyncTaskDialog);
  errorDialog->setWindowTitle(task.name());
  errorDialog->setText(error.what());
  errorDialog->setIcon(QMessageBox::Critical);
  errorDialog->setModal(true);
  errorDialog->show();
}
