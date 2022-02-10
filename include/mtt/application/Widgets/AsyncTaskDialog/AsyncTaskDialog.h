#pragma once

#include <vector>

#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>

#include <mtt/application/AsyncTasks/AsyncTaskQueue.h>

namespace mtt
{
  class TaskProgressWidget;

  class AsyncTaskDialog : public QDialog
  {
    Q_OBJECT

  public:
    explicit AsyncTaskDialog(AsyncTaskQueue& queue);
    AsyncTaskDialog(const AsyncTaskDialog&) = delete;
    AsyncTaskDialog& operator = (const AsyncTaskDialog&) = delete;
    virtual ~AsyncTaskDialog() noexcept = default;

  protected:
    virtual void showEvent(QShowEvent* theEvent) override;

  private:
    void _addTaskWidget(AbstractAsyncTask& task) noexcept;
    void _removeTaskWidget(AbstractAsyncTask& task) noexcept;
    void _adjustDialogSize() noexcept;
    void _processCounters(const AsyncTaskQueue::Counters& counters) noexcept;

  private:
    AsyncTaskQueue& _queue;
    QVBoxLayout _mainLayout;
    using TaskWidgets = std::vector<TaskProgressWidget*>;
    TaskWidgets _taskWidgets;
  };
};