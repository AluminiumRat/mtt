#include <mtt/Core/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/Core/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/Core/Widgets/AsyncTaskDialog/TaskProgressWidget.h>

#include <GeneratedFiles/ui_TaskProgressWidget.h>

using namespace mtt;

TaskProgressWidget::TaskProgressWidget( AsyncTaskQueue& queue,
                                        AbstractAsyncTask& task) :
  _ui(new Ui::TaskProgressWidget),
  _queue(queue),
  _task(task)
{
  _ui->setupUi(this);
  _ui->taskNameLabel->setText(_task.name());

  connect(&queue,
          &AsyncTaskQueue::stageStarted,
          this,
          &TaskProgressWidget::_updateStage,
          Qt::DirectConnection);
  _updateStage(_task, tr("Starting"));

  connect(&queue,
          &AsyncTaskQueue::percentCompleate,
          this,
          &TaskProgressWidget::_updatePercent,
          Qt::DirectConnection);
  _updatePercent(_task, 0);
}

TaskProgressWidget::~TaskProgressWidget() noexcept
{
  delete _ui;
}

void TaskProgressWidget::_updateStage(AbstractAsyncTask& task,
                                      const QString& stageName) noexcept
{
  if(&task != &_task) return;
  _ui->stageLabel->setText(stageName);
}

void TaskProgressWidget::_updatePercent(AbstractAsyncTask& task,
                                        int percent) noexcept
{
  if (&task != &_task) return;
  _ui->progressBar->setValue(percent);
}
