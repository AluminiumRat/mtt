#pragma once

#include <functional>
#include <memory>

#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/application/DrawModel/SlaveDrawModel.h>

namespace mtt
{
  class LoadEnvironmentModelTask : public AbstractAsyncTask
  {
  public:
    using Callback = std::function<void(std::unique_ptr<SlaveDrawModel>)>;

  public:
    LoadEnvironmentModelTask( const QString& filename,
                              const Callback& callback);
    LoadEnvironmentModelTask(const LoadEnvironmentModelTask&) = delete;
    LoadEnvironmentModelTask& operator = (
                                      const LoadEnvironmentModelTask&) = delete;
    virtual ~LoadEnvironmentModelTask() noexcept = default;

  protected:
    virtual void asyncPart() override;
    virtual void finalizePart() override;

  private:
    QString _filename;
    Callback _callback;
    std::unique_ptr<SlaveDrawModel> _model;
  };
}