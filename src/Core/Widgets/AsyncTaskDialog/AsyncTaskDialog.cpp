#include <mtt/Core/Widgets/AsyncTaskDialog/AsyncTaskDialog.h>
#include <mtt/Core/Widgets/AsyncTaskDialog/TaskProgressWidget.h>
#include <mtt/Utilities/Log.h>

using namespace mtt;

AsyncTaskDialog::AsyncTaskDialog(AsyncTaskQueue& queue) :
  _queue(queue)
{
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

  setLayout(&_mainLayout);

  connect(&_queue,
          &AsyncTaskQueue::taskStarted,
          this,
          &AsyncTaskDialog::_addTaskWidget);

  std::vector<AbstractAsyncTask*> currentTasks = _queue.tasksInProgress();
  for(AbstractAsyncTask* task : currentTasks) _addTaskWidget(*task);

  connect(&_queue,
          &AsyncTaskQueue::taskFinished,
          this,
          &AsyncTaskDialog::_removeTaskWidget);

  connect(&_queue,
          &AsyncTaskQueue::countersChanged,
          this,
          &AsyncTaskDialog::_processCounters);
  _processCounters(_queue.counters());

  _adjustDialogSize();
}

void AsyncTaskDialog::_addTaskWidget(AbstractAsyncTask& task) noexcept
{
  if(task.displayBehavior() == AbstractAsyncTask::SILENT) return;
  try
  {
    std::unique_ptr<TaskProgressWidget> newWidget(
                                          new TaskProgressWidget(_queue, task));
    _taskWidgets.push_back(newWidget.get());
    _mainLayout.addWidget(newWidget.release());
    _adjustDialogSize();
  }
  catch(std::exception& error)
  {
    Log() << "AsyncTaskDialog::addTaskWidget: " << error.what();
  }
  catch(...)
  {
    Log() << "AsyncTaskDialog::addTaskWidget: unknown error";
  }
}

void AsyncTaskDialog::_removeTaskWidget(AbstractAsyncTask& task) noexcept
{
  for(TaskWidgets::iterator iWidget = _taskWidgets.begin();
      iWidget != _taskWidgets.end();
      iWidget++) 
  {
    TaskProgressWidget* widget = *iWidget;
    if(&widget->task() == &task)
    {
      _taskWidgets.erase(iWidget);
      _mainLayout.removeWidget(widget);
      delete widget;
      _adjustDialogSize();
      return;
    }
  }
}

void AsyncTaskDialog::_adjustDialogSize() noexcept
{
  adjustSize();
  setFixedSize(size());
}

void AsyncTaskDialog::_processCounters(
                              const AsyncTaskQueue::Counters& counters) noexcept
{
  QString title = tr("Processed ") +
                    QString::number(counters.explicitFinishedInChunk) + "/" +
                    QString::number(counters.explicitInChunk);
  setWindowTitle(title);
}
