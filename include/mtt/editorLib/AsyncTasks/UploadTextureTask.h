#pragma once

#include <functional>
#include <memory>

#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/render/Pipeline/Texture2D.h>

namespace mtt
{
  class UploadTextureTask : public AbstractAsyncTask
  {
  public:
    using ApplyFunction = std::function<void(std::shared_ptr<Texture2D>)>;

  public:
    UploadTextureTask(const QString& filename,
                      const ApplyFunction& applyFunction);
    UploadTextureTask(const UploadTextureTask&) = delete;
    UploadTextureTask& operator = (const UploadTextureTask&) = delete;
    virtual ~UploadTextureTask() noexcept = default;

  protected:
    virtual void asyncPart() override;
    virtual void finalizePart() override;

  private:
    QString _filename;
    std::shared_ptr<Texture2D> _texture;
    ApplyFunction _applyFunction;
  };
}