#pragma once

#include <memory>
#include <vector>

#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/render/Pipeline/Texture2D.h>

class ParticlesDrawable;

class UploadParticleTexturesTask : public mtt::AbstractAsyncTask
{
public:
  UploadParticleTexturesTask( const std::vector<QString>& filenames,
                              ParticlesDrawable& drawable);
  UploadParticleTexturesTask(const UploadParticleTexturesTask&) = delete;
  UploadParticleTexturesTask& operator = (
                                    const UploadParticleTexturesTask&) = delete;
  virtual ~UploadParticleTexturesTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  std::vector<QString> _filenames;
  ParticlesDrawable& _drawable;
  std::vector<std::shared_ptr<mtt::Texture2D>> _textures;
};