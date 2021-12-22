#pragma once

#include <memory>

#include <mtt/Application/DrawModel/DrawModel.h>

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
  std::unique_ptr<mtt::DrawModel> _drawModel;
};