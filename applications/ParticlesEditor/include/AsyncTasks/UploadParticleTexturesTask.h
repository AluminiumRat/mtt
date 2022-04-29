#pragma once

#include <memory>
#include <vector>

#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/render/Pipeline/Texture2D.h>

#include <Objects/ParticleTextureDescription.h>
#include <Render/Particles/ParticlesDrawable.h>

class UploadParticleTexturesTask : public mtt::AbstractAsyncTask
{
public:
  UploadParticleTexturesTask( const ParticleTextureDescriptions& descriptions,
                              ParticlesDrawable& drawable);
  UploadParticleTexturesTask(const UploadParticleTexturesTask&) = delete;
  UploadParticleTexturesTask& operator = (
                                    const UploadParticleTexturesTask&) = delete;
  virtual ~UploadParticleTexturesTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  ParticleTextureDescriptions _descriptions;
  ParticlesDrawable& _drawable;

  std::vector<ParticlesDrawable::TextureData> _textures;
};