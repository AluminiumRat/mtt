#pragma once

#include <array>
#include <functional>
#include <memory>

#include <QtCore/QString>

#include <mtt/Application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/Render/Pipeline/CubeTexture.h>

namespace mtt
{
  class CubeTexture;
};

class UploadCubetextureTask : public mtt::AbstractAsyncTask
{
public:
  using Callback = std::function<void (std::shared_ptr<mtt::CubeTexture>)>;

public:
  UploadCubetextureTask(const std::array<QString,6>& filenames,
                        Callback&& callback);
  UploadCubetextureTask(const UploadCubetextureTask&) = delete;
  UploadCubetextureTask& operator = (const UploadCubetextureTask&) = delete;
  virtual ~UploadCubetextureTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  std::array<QString, 6> _filenames;
  std::shared_ptr<mtt::CubeTexture> _texture;
  Callback _callback;
};