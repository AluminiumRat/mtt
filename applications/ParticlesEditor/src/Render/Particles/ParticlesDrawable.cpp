#include <limits>

#include <mtt/application/Application.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/utilities/Abort.h>

#include <Render/Particles/ParticlesDrawable.h>

ParticlesDrawable::ParticlesDrawable() :
  _colorTechnique(_commonData),
  _shadowmapTechnique(_commonData)
{
}

void ParticlesDrawable::setData(const std::vector<glm::vec3>& positionData,
                                const std::vector<glm::vec2>& sizeRotationData,
                                const std::vector<glm::vec4>& albedoData,
                                const std::vector<glm::vec4>& emissionData,
                                const std::vector<uint32_t>& textureIndexData,
                                const std::vector<uint32_t>& tileIndexData)
{
  if( positionData.size() != sizeRotationData.size() ||
      positionData.size() != albedoData.size() ||
      positionData.size() != emissionData.size() ||
      positionData.size() != textureIndexData.size() ||
      positionData.size() != tileIndexData.size() ) mtt::Abort("ParticlesDrawable::setData: data vectors have different sizes");

  _commonData.particlesNumber = 0;

  size_t particlesNumber = positionData.size();
  if(particlesNumber == 0) return;

  particlesNumber = std::min( particlesNumber,
                              size_t(std::numeric_limits<uint16_t>::max()));

  _commonData.positionBuffer.setData( positionData.data(),
                                      particlesNumber * sizeof(glm::vec3));
  _commonData.sizeRotationBuffer.setData( sizeRotationData.data(),
                                          particlesNumber * sizeof(glm::vec2));
  _commonData.albedoBuffer.setData( albedoData.data(),
                                    particlesNumber * sizeof(glm::vec4));
  _commonData.emissionBuffer.setData( emissionData.data(),
                                      particlesNumber * sizeof(glm::vec4));
  _commonData.textureIndexBuffer.setData( textureIndexData.data(),
                                          particlesNumber * sizeof(uint32_t));
  _commonData.tileIndexBuffer.setData(tileIndexData.data(),
                                      particlesNumber * sizeof(uint32_t));

  _commonData.positionsData = positionData;
  _commonData.particlesNumber = particlesNumber;
}

void ParticlesDrawable::setParticleTextures(
                                      const std::vector<TextureData>& textures)
{
  _colorTechnique.clearPipelines();
  _shadowmapTechnique.clearPipeline();
  _commonData.textureSampler.reset();
  _commonData.textureData.clear();

  if(textures.empty()) return;

  try
  {
    _commonData.textureData = textures;
    _commonData.textureSampler.emplace(
                                  textures.size(),
                                  mtt::PipelineResource::STATIC,
                                  mtt::Application::instance().displayDevice());
    for ( size_t textureIndex = 0;
          textureIndex < textures.size();
          textureIndex++)
    {
      if(textures[textureIndex].texture == nullptr) mtt::Abort("ParticlesDrawable::setParticleTextures: textures array contain nullptr");
      _commonData.textureSampler->setAttachedTexture(
                                                textures[textureIndex].texture,
                                                textureIndex);
    }
  }
  catch (...)
  {
    _commonData.textureData.clear();
    _commonData.textureSampler.reset();
    throw;
  }
}

void ParticlesDrawable::buildDrawActions(mtt::DrawPlanBuildInfo& buildInfo)
{
  if(_commonData.particlesNumber == 0) return;
  if(buildInfo.frameType == mtt::clPipeline::colorFrameType)
  {
    _colorTechnique.addToDrawPlan(buildInfo);
  }
  if(buildInfo.frameType == mtt::clPipeline::shadowmapFrameType)
  {
    _shadowmapTechnique.addToDrawPlan(buildInfo);
  }
}

void ParticlesDrawable::registerAreaModificators(mtt::AreaModificatorSet& set)
{
  _colorTechnique.registerAreaModificators(set);
}

void ParticlesDrawable::unregisterAreaModificators(
                                          mtt::AreaModificatorSet& set) noexcept
{
  _colorTechnique.unregisterAreaModificators(set);
}
