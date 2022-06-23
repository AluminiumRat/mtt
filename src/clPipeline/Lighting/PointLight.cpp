#include <random>

#include <mtt/clPipeline/Lighting/CubeShadowmapProvider.h>
#include <mtt/clPipeline/Lighting/PointLight.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace clPipeline;

#define SAMPLE_CHUNKS_NUMBER 9

PointLight::PointLight( bool forwardLightingEnabled,
                        bool defferedLightingEnabled,
                        LogicalDevice& device) :
  _device(device),
  _shadowmapProvider(nullptr),
  _illuminance(1.f),
  _distance(50.f),
  _blurAngle(0.f)
{
  addChildProtected(_shadowmapFrontCamera);

  if(forwardLightingEnabled)
  {
    //_forwardLightApplicator.reset(new PointLightAreaModificator(*this));
    //addChildProtected(*_forwardLightApplicator);
  }

  if(defferedLightingEnabled)
  {
    _defferedLightApplicator.reset(new PointLightApplicator(*this, device));
    addChildProtected(*_defferedLightApplicator);
  }

  _updateBound();
}

void PointLight::setFilterTexture(std::shared_ptr<CubeTexture> newTexture)
{
  _resetPipelines();
  _filterSampler.reset();

  if(newTexture == nullptr) return;

  try
  {
    _filterSampler.emplace(1, PipelineResource::STATIC, _device);
    _filterSampler->setAttachedTexture(newTexture, 0);

    _filterSampler->setMinFilter(VK_FILTER_LINEAR);
    _filterSampler->setMagFilter(VK_FILTER_LINEAR);
    _filterSampler->setMipmapMode(VK_SAMPLER_MIPMAP_MODE_LINEAR);
    _filterSampler->setAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
    _filterSampler->setAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
  }
  catch (...)
  {
    _filterSampler.reset();
    throw;
  }
}

void PointLight::_updateShadowmapCamera() noexcept
{
  _shadowmapFrontCamera.setPerspectiveProjection( glm::pi<float>() / 2.f,
                                                  1,
                                                  distance() / 100.f,
                                                  distance());
}

void PointLight::_updateBound() noexcept
{
  if (_defferedLightApplicator != nullptr)
  {
    _defferedLightApplicator->updateBound();
  }
  //if (_forwardLightApplicator != nullptr)
  //{
  //  _forwardLightApplicator->updateBound();
  //}
}

void PointLight::_resetPipelines() noexcept
{
  if (_defferedLightApplicator != nullptr)
  {
    _defferedLightApplicator->resetPipelines();
  }
  //if (_forwardLightApplicator != nullptr) _forwardLightApplicator->reset();
}

void PointLight::setShadowmapProvider(
                                    CubeShadowmapProvider* newProvider) noexcept
{
  if (_shadowmapProvider == newProvider) return;

  _resetPipelines();
  _shadowmapSampler.reset();
  _blurShiftsBuffer.reset();

  _shadowmapProvider = newProvider;
  if(_shadowmapProvider == nullptr) return;

  try
  {
    _shadowmapSampler.emplace(1, PipelineResource::VOLATILE, _device);
    _shadowmapSampler->setAttachedTexture(std::make_shared<Texture2D>(_device),
                                          0);
    _shadowmapSampler->setMinFilter(VK_FILTER_LINEAR);
    _shadowmapSampler->setMagFilter(VK_FILTER_LINEAR);
    _shadowmapSampler->setMipmapMode(VK_SAMPLER_MIPMAP_MODE_NEAREST);
    _shadowmapSampler->setAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
    _shadowmapSampler->setAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

    _buildBlurShifts();
  }
  catch (...)
  {
    _shadowmapSampler.reset();
    _blurShiftsBuffer.reset();
    _shadowmapProvider = nullptr;
  }
}

void PointLight::_buildBlurShifts()
{
  std::default_random_engine gen;
  std::uniform_real_distribution<float> rnd(0.f, 1.f);

  _startShifts.resize(SAMPLE_CHUNKS_NUMBER);

  std::vector<glm::vec4> shiftsData;
  shiftsData.reserve(285);

  uint32_t valueIndex = 0;
  for (int chunk = 1; chunk <= SAMPLE_CHUNKS_NUMBER; chunk++)
  {
    _startShifts[chunk - 1] = valueIndex;

    float radiusRange = 1.f / chunk;
    float angleRange = 2.f * glm::pi<float>() / chunk;

    for (int layer = 0; layer < chunk; layer++)
    {
      for (int angleIndex = 0; angleIndex < chunk; angleIndex++)
      {
        if(chunk == 1) shiftsData.push_back(glm::vec4(0.f));
        else
        {
          float radius = radiusRange * (layer + rnd(gen));
          radius = sqrt(radius);
          float angle = angleRange * (angleIndex + rnd(gen));

          shiftsData.push_back(glm::vec4( radius * cos(angle),
                                          radius * sin(angle),
                                          0.f,
                                          0.f));
        }
        valueIndex++;
      }
    }
  }

  _blurShiftsBuffer.emplace(_device, Buffer::UNIFORM_BUFFER);
  _blurShiftsBuffer->setData( shiftsData.data(),
                              shiftsData.size() * sizeof(glm::vec4));
}

PointLightData PointLight::buildDrawData(
                              const DrawPlanBuildInfo& buildInfo) const noexcept
{
  PointLightData drawData;
  drawData.lightPosition = buildInfo.drawMatrices.localToViewMatrix *
                                                  glm::vec4(0.f, 0.f, 0.f, 1.f);
  drawData.illuminance = illuminance();
  drawData.distance = distance();
  drawData.clipToView = buildInfo.drawMatrices.clipToViewMatrix;
  drawData.viewToLocal = glm::inverse(buildInfo.drawMatrices.localToViewMatrix);
  drawData.blurRadius = 0.f;
  drawData.startSample = 0;
  drawData.endSample = 0;
  if (_shadowmapProvider != nullptr)
  {
    uint32_t extent = _shadowmapProvider->frameExtent();
    float mapBlurRadius = blurAngle() / glm::pi<float>();
    float radiusInTexel = mapBlurRadius * extent;
    float texelsNumber = glm::pi<float>() * radiusInTexel * radiusInTexel;

    uint32_t samplerChunkIndex = uint32_t(sqrt(texelsNumber / 2.f));
    //This is magic adjusting number                          ^^^

    samplerChunkIndex = std::min( samplerChunkIndex,
                                  uint32_t(SAMPLE_CHUNKS_NUMBER - 1));
    drawData.startSample = _startShifts[samplerChunkIndex];

    uint32_t samplesNumber = (samplerChunkIndex + 1) * (samplerChunkIndex + 1);
    drawData.endSample = drawData.startSample + samplesNumber;

    drawData.blurRadius = mapBlurRadius * 2.f;
  }

  return drawData;
}

size_t PointLight::culledDrawablesNumber() const noexcept
{
  return _defferedLightApplicator == nullptr ? 0 : 1;
}

DrawableNode& PointLight::culledDrawable(size_t index) noexcept
{
  if (_defferedLightApplicator == nullptr) Abort("PointLight::culledDrawable: no culled drawables available.");
  else return *_defferedLightApplicator;
}

size_t PointLight::areaModificatorsNumber() const noexcept
{
  return 0;//_forwardLightApplicator == nullptr ? 0 : 1;
}

AreaModificator& PointLight::areaModificator(size_t index) noexcept
{
  Abort("PointLight::areaModificator: no area modificators available.");
  //if (_forwardLightApplicator == nullptr) Abort("PointLight::areaModificator: no area modificators available.");
  //else return *_forwardLightApplicator;
}