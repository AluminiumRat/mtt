#include <mtt/clPipeline/Lighting/AmbientLight.h>

using namespace mtt;
using namespace clPipeline;

AmbientLight::AmbientLight(LogicalDevice& device) :
  _ambientMapSampler(PipelineResource::STATIC, device),
  _ambientMap(nullptr),
  _diffuseLuminanceSampler(PipelineResource::STATIC, device),
  _diffuseLuminanceMap(nullptr),
  _defferedApplicator(_lightData,
                      _ambientMapSampler,
                      _diffuseLuminanceSampler,
                      device),
  _forwardApplicator( _lightData,
                      _ambientMapSampler,
                      _diffuseLuminanceSampler)
{
  _lightData.illuminance = glm::vec3(1.f);
  _lightData.distance = 100.f;
  _lightData.saturationDistance = 0.f;
  _lightData.infinityAreaMode = false;
}

DrawableNode* AmbientLight::defferedLightApplicator() noexcept
{
  return &_defferedApplicator;
}

AreaModificator* AmbientLight::forwardLightModificator() noexcept
{
  return &_forwardApplicator;
}

void AmbientLight::_updateBound() noexcept
{
  _defferedApplicator.updateBound();
  _forwardApplicator.updateBound();
}

void AmbientLight::setAmbientMap(std::shared_ptr<CubeTexture> newMap) noexcept
{
  if (newMap.get() == _ambientMap) return;
  _ambientMap = newMap.get();
  _ambientMapSampler.setAttachedTexture(std::move(newMap), 0);
  _resetPipelines();
}

void AmbientLight::setDiffuseLuminanceMap(
                                  std::shared_ptr<CubeTexture> newMap) noexcept
{
  if(newMap.get() == _diffuseLuminanceMap) return;
  _diffuseLuminanceMap = newMap.get();
  _diffuseLuminanceSampler.setAttachedTexture(std::move(newMap), 0);
  _resetPipelines();
}

void AmbientLight::_resetPipelines() noexcept
{
  _defferedApplicator.resetPipelines();
  _forwardApplicator.reset();
}
