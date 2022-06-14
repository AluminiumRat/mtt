#include <mtt/clPipeline/Lighting/SpotLight.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace clPipeline;

SpotLight::SpotLight( bool forwardLightingEnabled,
                          bool defferedLightingEnabled,
                          LogicalDevice& device) :
  _device(device),
  _shadowMapProvider(nullptr),
  _illuminance(1.f),
  _distance(50.f),
  _angle(glm::pi<float>() / 4),
  _blurAngle(0.f)
{
  addChildProtected(_shadowmapCamera);

  if(forwardLightingEnabled)
  {
    //_forwardLightApplicator.reset(new SpotLightAreaModificator(*this));
    //addChildProtected(*_forwardLightApplicator);
  }

  if(defferedLightingEnabled)
  {
    _defferedLightApplicator.reset(new SpotLightApplicator(*this, device));
    addChildProtected(*_defferedLightApplicator);
  }

  _updateBound();
}

void SpotLight::setFilterTexture(std::shared_ptr<Texture2D> newTexture)
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

void SpotLight::_updateShadowmapCamera() noexcept
{
  _shadowmapCamera.setPerspectiveProjection(angle(),
                                            1,
                                            distance() / 100.f,
                                            distance());
}

void SpotLight::_updateBound() noexcept
{
  if (_defferedLightApplicator != nullptr)
  {
    _defferedLightApplicator->updateBound();
  }
  /*if (_forwardLightApplicator != nullptr)
  {
    _forwardLightApplicator->updateBound();
  }*/
}

void SpotLight::_resetPipelines() noexcept
{
  if (_defferedLightApplicator != nullptr)
  {
    _defferedLightApplicator->resetPipelines();
  }
  /*if (_forwardLightApplicator != nullptr) _forwardLightApplicator->reset(); */
}

void SpotLight::setShadowMapProvider(ShadowMapProvider* newProvider) noexcept
{
  if (_shadowMapProvider == newProvider) return;

  _resetPipelines();
  _shadowmapSampler.reset();

  _shadowMapProvider = newProvider;
  if(_shadowMapProvider == nullptr) return;

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
  }
  catch (...)
  {
    _shadowmapSampler.reset();
    _shadowMapProvider = nullptr;
  }
}

SpotLightData SpotLight::buildDrawData(
                              const DrawPlanBuildInfo& buildInfo) const noexcept
{
  SpotLightData drawData;
  drawData.lightPosition = buildInfo.drawMatrices.localToViewMatrix *
                                                  glm::vec4(0.f, 0.f, 0.f, 1.f);
  drawData.illuminance = illuminance();
  drawData.distance = distance();
  drawData.halfangleTan = tan(angle() / 2.f);
  drawData.clipToView = buildInfo.drawMatrices.clipToViewMatrix;
  drawData.viewToLocal = glm::inverse(buildInfo.drawMatrices.localToViewMatrix);
  if(angle() >= 0.f) drawData.blurRadius = blurAngle() / angle() / 2.f;
  else drawData.blurRadius = 0.f;

  return drawData;
}

size_t SpotLight::culledDrawablesNumber() const noexcept
{
  return _defferedLightApplicator == nullptr ? 0 : 1;
}

DrawableNode& SpotLight::culledDrawable(size_t index) noexcept
{
  if (_defferedLightApplicator == nullptr) Abort("SpotLight::culledDrawable: no culled drawables available.");
  else return *_defferedLightApplicator;
}

size_t SpotLight::areaModificatorsNumber() const noexcept
{
  return 0;//_forwardLightApplicator == nullptr ? 0 : 1;
}

AreaModificator& SpotLight::areaModificator(size_t index) noexcept
{
  Abort("SpotLight::areaModificator: no area modificators available.");
  //if (_forwardLightApplicator == nullptr) Abort("SpotLight::areaModificator: no area modificators available.");
  //else return *_forwardLightApplicator;
}