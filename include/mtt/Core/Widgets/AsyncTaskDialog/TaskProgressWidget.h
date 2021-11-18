#pragma once

#include <QtWidgets/QWidget>

namespace Ui
{
  class TaskProgressWidget;
}

namespace mtt
{
  class AbstractAsyncTask;
  class AsyncTaskQueue;

  class TaskProgressWidget : public QWidget
  {
    Q_OBJECT
  
  public:
    TaskProgressWidget(AsyncTaskQueue& queue, AbstractAsyncTask& task);
    TaskProgressWidget(const TaskProgressWidget&) = delete;
    TaskProgressWidget& operator = (const TaskProgressWidget&) = delete;
    virtual ~TaskProgressWidget() noexcept;
  
    inline AbstractAsyncTask& task() const noexcept;

  private:
    void _updateStage(AbstractAsyncTask& task,
                      const QString& stageName) noexcept;
    void _updatePercent(AbstractAsyncTask& task,
                        int percent) noexcept;

  private:
    Ui::TaskProgressWidget* _ui;
    AsyncTaskQueue& _queue;
    AbstractAsyncTask& _task;
  };

  inline AbstractAsyncTask& TaskProgressWidget::task() const noexcept
  {
    return _task;
  }
}