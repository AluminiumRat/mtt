#pragma once

#include <future>
#include <queue>
#include <memory>
#include <stdexcept>
#include <vector>

#include <QtCore/QObject>

#include <mtt/Core/AsyncTasks/AbstractAsyncTask.h>

namespace mtt
{
  class AsyncTaskQueue : public QObject
  {
    Q_OBJECT
  
  public:
    struct Counters
    {
      size_t chunkSize;               /// Total tasks in current chunk
      size_t finishedInChunk;         /// Number of finished taksk in current
                                      /// chunk
      size_t explicitInChunk;         /// Number of displayed tasks in current
                                      /// chunk
      size_t explicitFinishedInChunk; /// Number of finished displayed tasks in
                                      /// current chunk
    };

    class TaskStopper
    {
    private:
      friend class AsyncTaskQueue;
      inline TaskStopper( AsyncTaskQueue& queue,
                          AbstractAsyncTask& task) noexcept;

    public:
      TaskStopper(const TaskStopper&) = delete;
      TaskStopper& operator = (const TaskStopper&) = delete;
      inline ~TaskStopper() noexcept;

      inline AbstractAsyncTask* task() const noexcept;
      inline void abortTask() noexcept;
      inline void releaseTask() noexcept;

    private:
      AsyncTaskQueue& _queue;
      AbstractAsyncTask* _task;
    };

  public:
    AsyncTaskQueue();
    AsyncTaskQueue(const AsyncTaskQueue&) = delete;
    AsyncTaskQueue& operator = (const AsyncTaskQueue&) = delete;
    virtual ~AsyncTaskQueue() noexcept = default;

    void addTask(std::unique_ptr<AbstractAsyncTask> task);
    std::unique_ptr<TaskStopper> addTaskWithStopper(
                                      std::unique_ptr<AbstractAsyncTask> task);
    void abortTask(AbstractAsyncTask& task) noexcept;

    inline size_t maxTasksTogether() const noexcept;
    /// newValue should be greater than 0
    void setMaxTasksTogether(size_t newValue) noexcept;

    inline const Counters& counters() const noexcept;

    inline std::vector<AbstractAsyncTask*> tasksInProgress() const;

    /// Threadsafe method
    void reportStage( AbstractAsyncTask& task,
                      const QString& stageName) noexcept;

    /// Threadsafe method
    void reportPercent(AbstractAsyncTask& task, size_t percent) noexcept;

    /// Threadsafe method
    void reportWarning( AbstractAsyncTask& task,
                        const QString& message) noexcept;

  signals:
    void taskStarted(AbstractAsyncTask& task);
    void stageStarted(AbstractAsyncTask& task, const QString& stageName);
    void percentCompleate(AbstractAsyncTask& task, int percent);
    void warningEmitted(AbstractAsyncTask& task, const QString& message);
    void taskFinished(AbstractAsyncTask& task);
    void errorEmitted(AbstractAsyncTask& task,const std::exception& error);
    void countersChanged(const Counters& counters);

  protected:
    virtual bool event(QEvent* theEvent) override;
  
  private:
    void _reportFinish(AbstractAsyncTask& task) noexcept;
    void _finalizeTask(AbstractAsyncTask& task) noexcept;
    void _startTasks() noexcept;
    bool _prepareTask(AbstractAsyncTask& task) noexcept;
    void _checkCounters() noexcept;
    void _releaseStopper(TaskStopper& stopper) noexcept;
    void _invalidateStopper(AbstractAsyncTask& task) noexcept;

  private:
    struct StageEvent;
    struct PercentEvent;
    struct WarningEvent;
    struct FinishEvent;
  
  private:
    size_t _maxTasksTogether;

    using WaitQueue = std::queue<std::unique_ptr<AbstractAsyncTask>>;
    WaitQueue _waitQueue;

    using TaskList = std::vector<std::unique_ptr<AbstractAsyncTask>>;
    TaskList _inProgress;
    bool _dependentTaskInProcess;
    using Futures = std::vector<std::future<void>>;
    Futures _futures;

    using Stoppers = std::vector<TaskStopper*>;
    Stoppers _stoppers;

    Counters _counters;
  };

  inline AsyncTaskQueue::TaskStopper::TaskStopper(
                                            AsyncTaskQueue& queue,
                                            AbstractAsyncTask& task) noexcept :
    _queue(queue),
    _task(&task)
  {
  }

  inline AsyncTaskQueue::TaskStopper::~TaskStopper() noexcept
  {
    abortTask();
    _queue._releaseStopper(*this);
  }

  inline AbstractAsyncTask* AsyncTaskQueue::TaskStopper::task() const noexcept
  {
    return _task;
  }

  inline void AsyncTaskQueue::TaskStopper::abortTask() noexcept
  {
    if (_task != nullptr) _queue.abortTask(*_task);
  }

  inline void AsyncTaskQueue::TaskStopper::releaseTask() noexcept
  {
    _task = nullptr;
  }

  inline size_t AsyncTaskQueue::maxTasksTogether() const noexcept
  {
    return _maxTasksTogether;
  }

  inline const AsyncTaskQueue::Counters&
                                      AsyncTaskQueue::counters() const noexcept
  {
    return _counters;
  }

  inline std::vector<AbstractAsyncTask*> AsyncTaskQueue::tasksInProgress() const
  {
    std::vector<AbstractAsyncTask*> tasks;
    for(const std::unique_ptr<AbstractAsyncTask>& task : _inProgress)
    {
      tasks.push_back(task.get());
    }
    return tasks;
  }
}