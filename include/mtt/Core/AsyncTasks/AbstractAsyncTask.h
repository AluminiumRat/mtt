#pragma once

#include <atomic>

#include <QtCore/QString>

namespace mtt
{
  class AsyncTaskQueue;

  class AbstractAsyncTask
  {
  public:
    enum Relationship
    {
      DEPENDENT,    /// Cannot be executed in parallel with other depended tasks
      INDEPENDENT   /// Can be executed in parallel with any other tasks
    };

    enum DisplayBehavior
    {
      EXPLICIT,     /// The task is displayed in the AsyncTaskDialog
      SILENT        /// The task is not displayed in the AsyncTaskDialog
    };

  public:
    AbstractAsyncTask(const QString& name,
                      Relationship relationship,
                      DisplayBehavior displayBehavior);
    AbstractAsyncTask(const AbstractAsyncTask&) = delete;
    AbstractAsyncTask& operator = (const AbstractAsyncTask&) = delete;
    virtual ~AbstractAsyncTask() noexcept = default;
  
    inline const QString& name() const noexcept;
    inline Relationship relationship() const noexcept;
    inline DisplayBehavior displayBehavior() const noexcept;

    void prepare(AsyncTaskQueue& queue);
    void make();
    void finalize();

    inline void notifyAbort() noexcept;
    inline bool aborted() const noexcept;

  protected:
    virtual void preparePart();
    virtual void asyncPart();
    virtual void finalizePart();

  protected:
    void reportStage(const QString& stageName) noexcept;
    void reportPercent(size_t percent) noexcept;
    void reportWarning(const QString& message) noexcept;

  private:
    QString _name;
    Relationship _relationship;
    DisplayBehavior _displayBehavior;
    AsyncTaskQueue* _queue;
    size_t _lastPercent;

    std::atomic<bool> _aborted;
  };

  inline const QString& AbstractAsyncTask::name() const noexcept
  {
    return _name;
  }

  inline AbstractAsyncTask::Relationship
                                AbstractAsyncTask::relationship() const noexcept
  {
    return _relationship;
  }

  inline AbstractAsyncTask::DisplayBehavior
                            AbstractAsyncTask::displayBehavior() const noexcept
  {
    return _displayBehavior;
  }

  inline void AbstractAsyncTask::notifyAbort() noexcept
  {
    _aborted.store(true);
  }

  inline bool AbstractAsyncTask::aborted() const noexcept
  {
    return _aborted.load();
  }
}