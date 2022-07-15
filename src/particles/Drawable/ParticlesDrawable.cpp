#include <limits>
#include <random>

#include <mtt/clPipeline/constants.h>
#include <mtt/particles/Drawable/ParticlesDrawable.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

ParticlesDrawable::ParticlesDrawable(LogicalDevice& device) :
  _commonData(device),
  _colorTechnique(_commonData),
  _shadowmapTechnique(_commonData)
{
  std::default_random_engine randomEngine;
  std::uniform_real_distribution<float> distribution(0.001f, 1.f);
  for (float& value : _randomFactorTable)
  {
    value = 1.f / distribution(randomEngine);
  }
}

void ParticlesDrawable::setData(const std::vector<glm::vec3>& positionData,
                                const std::vector<glm::vec2>& sizeRotationData,
                                const std::vector<glm::vec4>& albedoData,
                                const std::vector<glm::vec4>& emissionData,
                                const std::vector<uint32_t>& textureIndexData,
                                const std::vector<uint32_t>& tileIndexData,
                                const std::vector<uint8_t>& tagData)
{
  if( positionData.size() != sizeRotationData.size() ||
      positionData.size() != albedoData.size() ||
      positionData.size() != emissionData.size() ||
      positionData.size() != textureIndexData.size() ||
      positionData.size() != tileIndexData.size() ||
      positionData.size() != tagData.size()) Abort("ParticlesDrawable::setData: data vectors have different sizes");

  _commonData.particlesNumber = 0;

  size_t particlesNumber = positionData.size();
  if(particlesNumber == 0) return;

  particlesNumber = std::min( particlesNumber,
                              size_t(std::numeric_limits<uint16_t>::max()));

  std::vector<float> falloffFactorData;
  falloffFactorData.reserve(particlesNumber);
  for ( size_t particleIndex = 0;
        particleIndex < particlesNumber;
        particleIndex++)
  {
    falloffFactorData.push_back(_randomFactorTable[tagData[particleIndex]]);
  }

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
  _commonData.falloffFactorBuffer.setData(falloffFactorData.data(),
                                          particlesNumber * sizeof(float));

  _commonData.positionsData = positionData;
  _commonData.falloffFactorData = std::move(falloffFactorData);
  _commonData.tagData = tagData;
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
    _commonData.textureSampler.emplace( textures.size(),
                                        PipelineResource::STATIC,
                                        _commonData.device);
    for ( size_t textureIndex = 0;
          textureIndex < textures.size();
          textureIndex++)
    {
      if(textures[textureIndex].texture == nullptr) Abort("ParticlesDrawable::setParticleTextures: textures array contain nullptr");
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

void ParticlesDrawable::setFalloffBaseMppx(float newValue) noexcept
{
  _commonData.falloffBaseMppx = std::max(newValue, 0.f);
}

void ParticlesDrawable::setFalloffSmoothing(float newValue) noexcept
{
  _commonData.falloffSmoothing = std::max(newValue, 0.f);
}

void ParticlesDrawable::setLightingType(
                              ParticlesDrawable::LightingType newValue) noexcept
{
  if(newValue == lightingType()) return;
  _commonData.lightingType =
                  static_cast<ParticlesDrawCommonData::LightingType>(newValue);
  _colorTechnique.clearPipelines();
  _shadowmapTechnique.clearPipeline();
}

void ParticlesDrawable::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  if(_commonData.particlesNumber == 0) return;
  if(buildInfo.frameType == clPipeline::colorFrameType)
  {
    _colorTechnique.addToDrawPlan(buildInfo);
  }
  if(buildInfo.frameType == clPipeline::transparentShadowmapFrameType)
  {
    _shadowmapTechnique.addToDrawPlan(buildInfo);
  }
}

void ParticlesDrawable::registerAreaModificators(AreaModificatorSet& set)
{
  _colorTechnique.registerAreaModificators(set);
}

void ParticlesDrawable::unregisterAreaModificators(
                                              AreaModificatorSet& set) noexcept
{
  _colorTechnique.unregisterAreaModificators(set);
}
