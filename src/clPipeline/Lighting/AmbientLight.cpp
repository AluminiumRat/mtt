#include <mtt/clPipeline/Lighting/AmbientLight.h>

using namespace mtt;
using namespace clPipeline;

AmbientLight::AmbientLight( bool forwardLightingEnabled,
                            bool defferedLightingEnabled,
                            LogicalDevice& device) :
  _ambientMapSampler(PipelineResource::STATIC, device),
  _ambientMap(nullptr),
  _diffuseLuminanceSampler(PipelineResource::STATIC, device),
  _diffuseLuminanceMap(nullptr)
{
  if (forwardLightingEnabled)
  {
    _forwardApplicator.reset(new AmbientLightAreaModificator(
                                                    _lightData,
                                                    _ambientMapSampler,
                                                    _diffuseLuminanceSampler));
  }

  if (defferedLightingEnabled)
  {
    _defferedApplicator.reset(new AmbientLightApplicator(
                                                      _lightData,
                                                      _ambientMapSampler,
                                                      _diffuseLuminanceSampler,
                                                      device));
  }

  _lightData.illuminance = glm::vec3(1.f);
  _lightData.distance = 100.f;
  _lightData.saturationDistance = 0.f;
  _lightData.infinityAreaMode = false;

  _updateBound();
}

DrawableNode* AmbientLight::defferedLightApplicator() noexcept
{
  return _defferedApplicator.get();
}

AreaModificator* AmbientLight::forwardLightModificator() noexcept
{
  return _forwardApplicator.get();
}

void AmbientLight::_updateBound() noexcept
{
  if (_defferedApplicator != nullptr)
  {
    _defferedApplicator->updateBound();
  }

  if (_forwardApplicator != nullptr)
  {
    _forwardApplicator->updateBound();
  }
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
  if (_defferedApplicator != nullptr) _defferedApplicator->resetPipelines();
  if (_forwardApplicator != nullptr) _forwardApplicator->reset();
}
