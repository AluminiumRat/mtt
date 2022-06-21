#include <random>

#include <mtt/clPipeline/Lighting/PointLight.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace clPipeline;

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

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.f, 1.f);
    std::vector<glm::vec2> shiftsData;
    shiftsData.reserve(1024);
    for (int i = 0; i < 1024; i++)
    {
      float radius = sqrt(distribution(generator));
      float angle = 2.f * glm::pi<float>() * distribution(generator);
      shiftsData.push_back(glm::vec2( radius * cos(angle),
                                      radius * sin(angle)));
    }
    _blurShiftsBuffer.emplace(_device, Buffer::UNIFORM_BUFFER);
    _blurShiftsBuffer->setData( shiftsData.data(),
                                shiftsData.size() * sizeof(glm::vec2));
  }
  catch (...)
  {
    _shadowmapSampler.reset();
    _blurShiftsBuffer.reset();
    _shadowmapProvider = nullptr;
  }
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
  drawData.sampleNumber = 1;
  if (_shadowmapSampler.has_value())
  {
    uint32_t extent = static_cast<CubeTexture*>(
                          _shadowmapSampler->attachedTexture(0))->sideExtent();
    float mapBlurRadius = blurAngle() / glm::pi<float>();
    float radiusInTexel = mapBlurRadius * extent;
    float texelsNumber = glm::pi<float>() * radiusInTexel * radiusInTexel;
    float samplesNumber = texelsNumber / 4.f;
    drawData.sampleNumber = glm::clamp(int(samplesNumber), 1, 1024);
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