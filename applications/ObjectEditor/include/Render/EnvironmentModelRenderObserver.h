#pragma once

#include <memory>

#include <mtt/application/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/application/DrawModel/SlaveDrawModel.h>

#include <Render/Object3DRenderObserver.h>

class EnvironmentModel;

class EnvironmentModelRenderObserver : public Object3DRenderObserver
{
  Q_OBJECT

public:
  EnvironmentModelRenderObserver( EnvironmentModel& object,
                                  EditorCommonData& commonData);
  EnvironmentModelRenderObserver(
                              const EnvironmentModelRenderObserver&) = delete;
  EnvironmentModelRenderObserver& operator = (
                              const EnvironmentModelRenderObserver&) = delete;
  virtual ~EnvironmentModelRenderObserver() noexcept = default;

private:
  void _updateModel() noexcept;

private:
  EnvironmentModel& _object;
  std::unique_ptr<mtt::SlaveDrawModel> _drawModel;

  std::unique_ptr<mtt::AsyncTaskQueue::TaskStopper> _uploadStopper;
};