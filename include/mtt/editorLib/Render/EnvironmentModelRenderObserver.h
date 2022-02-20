#pragma once

#include <memory>

#include <mtt/application/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/application/DrawModel/SlaveDrawModel.h>
#include <mtt/editorLib/Render/Object3DRenderObserver.h>

namespace mtt
{
  class EnvironmentModel;

  class EnvironmentModelRenderObserver : public Object3DRenderObserver
  {
    Q_OBJECT

  public:
    EnvironmentModelRenderObserver( EnvironmentModel& object,
                                    CommonEditData& commonData);
    EnvironmentModelRenderObserver(
                                const EnvironmentModelRenderObserver&) = delete;
    EnvironmentModelRenderObserver& operator = (
                                const EnvironmentModelRenderObserver&) = delete;
    virtual ~EnvironmentModelRenderObserver() noexcept = default;

  private:
    void _updateModel() noexcept;

  private:
    EnvironmentModel& _object;
    std::unique_ptr<SlaveDrawModel> _drawModel;

    std::unique_ptr<AsyncTaskQueue::TaskStopper> _uploadStopper;
  };
}