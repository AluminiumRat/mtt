#pragma once

#include <memory>
#include <vector>

#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/clPipeline/Particles/ParticlesDrawable.h>
#include <mtt/render/Pipeline/Texture2D.h>

#include <Objects/ParticleTextureDescription.h>

class UploadParticleTexturesTask : public mtt::AbstractAsyncTask
{
public:
  UploadParticleTexturesTask( const ParticleTextureDescriptions& descriptions,
                              mtt::clPipeline::ParticlesDrawable& drawable);
  UploadParticleTexturesTask(const UploadParticleTexturesTask&) = delete;
  UploadParticleTexturesTask& operator = (
                                    const UploadParticleTexturesTask&) = delete;
  virtual ~UploadParticleTexturesTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  ParticleTextureDescriptions _descriptions;
  mtt::clPipeline::ParticlesDrawable& _drawable;

  std::vector<mtt::clPipeline::ParticlesDrawable::TextureData> _textures;
};