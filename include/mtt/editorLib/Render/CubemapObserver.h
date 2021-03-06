#pragma once

#include <functional>
#include <memory>

#include <QtCore/QObject>

#include <mtt/application/AsyncTasks/AsyncTaskQueue.h>
#include <mtt/render/Pipeline/CubeTexture.h>

namespace mtt
{
  class CubemapObject;
  class CubeTexture;

  class CubemapObserver : public QObject
  {
    Q_OBJECT

  public:
    using Callback = std::function<void(std::shared_ptr<CubeTexture>)>;

  public:
    CubemapObserver(CubemapObject& object);
    CubemapObserver(const CubemapObserver&) = delete;
    CubemapObserver& operator = (const CubemapObserver&) = delete;
    virtual ~CubemapObserver() = default;

    void setCallback(Callback&& newCallback) noexcept;

  private:
    void _updateTexture() noexcept;

  private:
    CubemapObject& _object;
    Callback _callback;

    using StopperUniquePtr = std::unique_ptr<AsyncTaskQueue::TaskStopper>;
    StopperUniquePtr _uploadStopper;
  };
}