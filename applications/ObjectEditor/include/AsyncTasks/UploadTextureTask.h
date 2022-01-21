#pragma once

#include <memory>
#include <string>

#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/render/Pipeline/Sampler.h>

namespace mtt
{
  class MeshExtraData;
}

class UploadTextureTask : public mtt::AbstractAsyncTask
{
public:
  UploadTextureTask(const QString& filename,
                    mtt::MeshExtraData& dstData,
                    const char* samplerName);
  UploadTextureTask(const UploadTextureTask&) = delete;
  UploadTextureTask& operator = (const UploadTextureTask&) = delete;
  virtual ~UploadTextureTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  QString _filename;
  mtt::MeshExtraData& _dstData;
  std::string _samplerName;
  std::unique_ptr<mtt::Sampler> _sampler;
};