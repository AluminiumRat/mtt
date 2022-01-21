#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/application/AsyncTasks/AsyncTaskQueue.h>

using namespace mtt;

AbstractAsyncTask::AbstractAsyncTask( const QString& name,
                                      Relationship relationship,
                                      DisplayBehavior displayBehavior) :
  _name(name),
  _relationship(relationship),
  _displayBehavior(displayBehavior),
  _queue(nullptr),
  _lastPercent(0),
  _aborted(false)
{
}

void AbstractAsyncTask::prepare(AsyncTaskQueue& queue)
{
  if(aborted()) return;
  _queue = &queue;
  preparePart();
}

void AbstractAsyncTask::preparePart()
{
}

void AbstractAsyncTask::make()
{
  if(aborted()) return;
  asyncPart();
}

void AbstractAsyncTask::asyncPart()
{
}

void AbstractAsyncTask::finalize()
{
  if(aborted()) return;
  finalizePart();
}

void AbstractAsyncTask::finalizePart()
{
}

void AbstractAsyncTask::reportStage(const QString& stageName) noexcept
{
  if(_queue == nullptr) return;
  _queue->reportStage(*this, stageName);
  reportPercent(0);
}

void AbstractAsyncTask::reportPercent(size_t percent) noexcept
{
  if(_queue == nullptr) return;
  if(_lastPercent == percent) return;
  _lastPercent = percent;
  _queue->reportPercent(*this, percent);
}

void AbstractAsyncTask::reportWarning(const QString& message) noexcept
{
  if(_queue == nullptr) return;
  _queue->reportWarning(*this, message);
}
