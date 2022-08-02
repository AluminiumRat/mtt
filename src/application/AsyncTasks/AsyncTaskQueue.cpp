#include <algorithm>
#include <stdexcept>

#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>

#include <mtt/application/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

static constexpr size_t DEFAULT_MAX_TASKS_TOGETHER = 5;

static const QEvent::Type ASYNC_TASK_STAGE_EVENT =
                                      (QEvent::Type)QEvent::registerEventType();
static const QEvent::Type ASYNC_TASK_PERCENT_EVENT =
                                      (QEvent::Type)QEvent::registerEventType();
static const QEvent::Type ASYNC_TASK_WARNING_EVENT =
                                      (QEvent::Type)QEvent::registerEventType();
static const QEvent::Type ASYNC_TASK_FINISH_EVENT =
                                      (QEvent::Type)QEvent::registerEventType();

struct AsyncTaskQueue::StageEvent : public QEvent
{
  AbstractAsyncTask& task;
  QString stageName;

  StageEvent(AbstractAsyncTask& theTask, const QString& newStageName) :
    QEvent(ASYNC_TASK_STAGE_EVENT),
    task(theTask),
    stageName(newStageName)
  {
  }
};

struct AsyncTaskQueue::PercentEvent : public QEvent
{
  AbstractAsyncTask& task;
  int percent;

  PercentEvent(AbstractAsyncTask& theTask, int newPercent) :
    QEvent(ASYNC_TASK_PERCENT_EVENT),
    task(theTask),
    percent(newPercent)
  {
  }
};

struct AsyncTaskQueue::WarningEvent : public QEvent
{
  AbstractAsyncTask& task;
  QString message;

  WarningEvent(AbstractAsyncTask& theTask, const QString& newMessage) :
    QEvent(ASYNC_TASK_WARNING_EVENT),
    task(theTask),
    message(newMessage)
  {
  }
};

struct AsyncTaskQueue::FinishEvent : public QEvent
{
  AbstractAsyncTask& task;

  FinishEvent(AbstractAsyncTask& theTask) :
    QEvent(ASYNC_TASK_FINISH_EVENT),
    task(theTask)
  {
  }
};

AsyncTaskQueue::AsyncTaskQueue() :
  _maxTasksTogether(DEFAULT_MAX_TASKS_TOGETHER),
  _dependentTaskInProcess(false),
  _counters{}
{
}

void AsyncTaskQueue::reportStage( AbstractAsyncTask& task,
                                  const QString& stageName) noexcept
{
  try
  {
    QCoreApplication::postEvent(this, new StageEvent(task, stageName));
  }
  catch(std::exception& error)
  {
    Log() << "AsyncTaskQueue::reportStage: " << error.what();
  }
  catch(...)
  {
    Log() << "AsyncTaskQueue::reportStage: unknown error";
  }
}

void AsyncTaskQueue::reportPercent( AbstractAsyncTask& task,
                                    size_t percent) noexcept
{
  try
  {
    QCoreApplication::postEvent(this, new PercentEvent(task, int(percent)));
  }
  catch(std::exception& error)
  {
    Log() << "AsyncTaskQueue::reportPercent: " << error.what();
  }
  catch(...)
  {
    Log() << "AsyncTaskQueue::reportPercent: unknown error";
  }
}

void AsyncTaskQueue::reportWarning( AbstractAsyncTask& task,
                                    const QString& message) noexcept
{
  try
  {
    QCoreApplication::postEvent(this, new WarningEvent(task, message));
  }
  catch(std::exception& error)
  {
    Log() << "AsyncTaskQueue::reportWarning: " << error.what();
  }
  catch(...)
  {
    Log() << "AsyncTaskQueue::reportWarning: unknown error";
  }
}

void AsyncTaskQueue::_reportFinish(AbstractAsyncTask& task) noexcept
{
  try
  {
    QCoreApplication::postEvent(this, new FinishEvent(task));
  }
  catch(std::exception& error)
  {
    Log() << "AsyncTaskQueue::reportFinish: " << error.what();
    Abort("AsyncTaskQueue::reportFinish: unable to send finish event.");
  }
  catch(...)
  {
    Log() << "AsyncTaskQueue::reportFinish: unknown error";
    Abort("AsyncTaskQueue::reportFinish: unable to send finish event.");
  }
}

bool AsyncTaskQueue::event(QEvent* theEvent)
{
  if(theEvent->type() == ASYNC_TASK_STAGE_EVENT)
  {
    StageEvent& stageEvent = static_cast<StageEvent&>(*theEvent);
    emit stageStarted(stageEvent.task, stageEvent.stageName);
    return true;
  }
  else if(theEvent->type() == ASYNC_TASK_PERCENT_EVENT)
  {
    PercentEvent& percentEvent = static_cast<PercentEvent&>(*theEvent);
    emit percentCompleate(percentEvent.task, percentEvent.percent);
    return true;
  }
  else if(theEvent->type() == ASYNC_TASK_WARNING_EVENT)
  {
    WarningEvent& warningEvent = static_cast<WarningEvent&>(*theEvent);
    emit warningEmitted(warningEvent.task, warningEvent.message);
    return true;
  }
  else if(theEvent->type() == ASYNC_TASK_FINISH_EVENT)
  {
    FinishEvent& finishEvent = static_cast<FinishEvent&>(*theEvent);
    _finalizeTask(finishEvent.task);
    return true;
  }

  return QObject::event(theEvent);
}

void AsyncTaskQueue::_finalizeTask(AbstractAsyncTask& task) noexcept
{
  for(size_t taskIndex = 0; taskIndex < _inProgress.size(); taskIndex++)
  {
    if(_inProgress[taskIndex].get() == &task)
    {
      std::unique_ptr<AbstractAsyncTask> taskPtr =
                                              std::move(_inProgress[taskIndex]);
      _inProgress.erase(_inProgress.begin() + taskIndex);
      std::future<void> future = std::move(_futures[taskIndex]);
      _futures.erase(_futures.begin() + taskIndex);

      if (task.relationship() == AbstractAsyncTask::DEPENDENT)
      {
        _dependentTaskInProcess = false;
      }

      bool exceptionEmitted = false;
      try
      {
        future.get();
      }
      catch(std::exception& error)
      {
        emit errorEmitted(task, error);
        exceptionEmitted = true;
      }
      catch(...)
      {
        emit errorEmitted(task, std::runtime_error("AsyncTaskQueue: Unknown error"));
        exceptionEmitted = true;
      }
      
      try
      {
        if(!exceptionEmitted && !task.aborted()) task.finalize();
        else task.restoreState();
      }
      catch(std::exception& error)
      {
        exceptionEmitted = true;
        emit errorEmitted(task, error);
      }
      catch(...)
      {
        exceptionEmitted = true;
        emit errorEmitted(task, std::runtime_error("AsyncTaskQueue: Unknown error"));
      }

      if(exceptionEmitted || task.aborted()) task.restoreState();

      _invalidateStopper(task);

      _counters.finishedInChunk++;
      if(task.displayBehavior() == AbstractAsyncTask::EXPLICIT)
      {
        _counters.explicitFinishedInChunk++;
      }

      emit taskFinished(task);
      emit countersChanged(_counters);

      _startTasks();
      _checkCounters();

      return;
    }
  }

  Abort("AsyncTaskQueue::_finalizeTask: task not found");
}

bool AsyncTaskQueue::_prepareTask(AbstractAsyncTask& task) noexcept
{
  try
  {
    if(!task.aborted()) task.prepare(*this);
    else
    {
      _invalidateStopper(task);
      _counters.finishedInChunk++;
      if (task.displayBehavior() == AbstractAsyncTask::EXPLICIT)
      {
        _counters.explicitFinishedInChunk++;
      }
      return false;
    }
  }
  catch (std::exception& error)
  {
    _invalidateStopper(task);
    _counters.finishedInChunk++;
    if (task.displayBehavior() == AbstractAsyncTask::EXPLICIT)
    {
      _counters.explicitFinishedInChunk++;
    }

    emit errorEmitted(task, error);
    emit taskFinished(task);
    emit countersChanged(_counters);

    return false;
  }
  catch (...)
  {
    _invalidateStopper(task);
    _counters.finishedInChunk++;
    if (task.displayBehavior() == AbstractAsyncTask::EXPLICIT)
    {
      _counters.explicitFinishedInChunk++;
    }

    emit errorEmitted(task, std::runtime_error("Unknown error"));
    emit taskFinished(task);
    emit countersChanged(_counters);

    return false;
  }

  return true;
}

void AsyncTaskQueue::_startTasks() noexcept
{
  while(!_waitQueue.empty() && _inProgress.size() < _maxTasksTogether)
  {
    if( _dependentTaskInProcess == true &&
        _waitQueue.front()->relationship() == AbstractAsyncTask::DEPENDENT)
    {
      return;
    }
  
    std::unique_ptr<AbstractAsyncTask> task = std::move(_waitQueue.front());
    _waitQueue.pop();

    emit taskStarted(*task);

    if(!_prepareTask(*task)) continue;
  
    if(task->relationship() == AbstractAsyncTask::DEPENDENT)
    {
      _dependentTaskInProcess = true;
    }

    AbstractAsyncTask* taskPtr = task.get();
    auto executor = [&, taskPtr]()
    {
      try
      {
        if(!taskPtr->aborted()) taskPtr->make();
      }
      catch(...)
      {
        _reportFinish(*taskPtr);
        throw;
      }
      _reportFinish(*taskPtr);
    };

    try
    {
      _inProgress.push_back(std::move(task));
      _futures.push_back(std::async(std::launch::async, executor));
    }
    catch(...)
    {
      Abort("AsyncTaskQueue::_startTasks: unable to start task.");
    }
  }
}

void AsyncTaskQueue::_checkCounters() noexcept
{
  if (_waitQueue.empty() && _inProgress.empty())
  {
    _counters.chunkSize = 0;
    _counters.explicitInChunk = 0;
    _counters.finishedInChunk = 0;
    _counters.explicitFinishedInChunk = 0;
    emit countersChanged(_counters);
  }
}

void AsyncTaskQueue::addTask(std::unique_ptr<AbstractAsyncTask> task)
{
  AbstractAsyncTask* taskPtr = task.get();
  _waitQueue.push(std::move(task));

  _counters.chunkSize++;
  if (taskPtr->displayBehavior() == AbstractAsyncTask::EXPLICIT)
  {
    _counters.explicitInChunk++;
  }
  emit countersChanged(_counters);

  _startTasks();
  _checkCounters();
}

std::unique_ptr<AsyncTaskQueue::TaskStopper>
                            AsyncTaskQueue::addTaskWithStopper(
                                      std::unique_ptr<AbstractAsyncTask> task)
{
  std::unique_ptr<TaskStopper> stopper(new TaskStopper(*this, *task));
  try
  {
    _stoppers.push_back(stopper.get());
    addTask(std::move(task));
  }
  catch(std::exception& error)
  {
    Log() << "AsyncTaskQueue::addTaskWithStopper: " << error.what();
    Abort("AsyncTaskQueue::addTaskWithStopper: unable to add task.");
  }
  catch(...)
  {
    Abort("AsyncTaskQueue::addTaskWithStopper: unable to add task.");
  }

  return stopper;
}

void AsyncTaskQueue::abortTask(AbstractAsyncTask& task) noexcept
{
  task.notifyAbort();
  _invalidateStopper(task);
  for(size_t taskIndex = 0; taskIndex < _inProgress.size(); taskIndex++)
  {
    if (_inProgress[taskIndex].get() == &task)
    {
      try
      {
        _futures[taskIndex].wait();
      }
      catch (std::exception& error)
      {
        Log() << "AsyncTaskQueue::abortTask: " << error.what();
        Abort("AsyncTaskQueue::abortTask: unable to wait async task.");
      }
      catch (...)
      {
        Abort("AsyncTaskQueue::abortTask: unable to wait async task.");
      }

      break;
    }
  }
}

void AsyncTaskQueue::_invalidateStopper(AbstractAsyncTask& task) noexcept
{
  for(TaskStopper* stopper : _stoppers)
  {
    if(stopper->task() == &task)
    {
      stopper->releaseTask();
      return;
    }
  }
}

void AsyncTaskQueue::_releaseStopper(TaskStopper& stopper) noexcept
{
  try
  {
    Stoppers::iterator iStopper = std::find(_stoppers.begin(),
                                            _stoppers.end(),
                                            &stopper);
    if(iStopper != _stoppers.end()) _stoppers.erase(iStopper);
  }
  catch (std::exception& error)
  {
    Log() << "AsyncTaskQueue::_releaseStopper: " << error.what();
    Abort("AsyncTaskQueue::_releaseStopper: unable to release stopper.");
  }
  catch (...)
  {
    Abort("AsyncTaskQueue::_releaseStopper: unable to release stopper.");
  }
}

void AsyncTaskQueue::setMaxTasksTogether(size_t newValue) noexcept
{
  if(newValue == 0) Abort("AsyncTaskQueue::setMaxTasksTogether: newValue == 0");
  _maxTasksTogether = newValue;
}
