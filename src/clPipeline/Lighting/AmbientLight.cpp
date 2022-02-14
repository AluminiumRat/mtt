#include <mtt/clPipeline/Lighting/AmbientLight.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace clPipeline;

AmbientLight::AmbientLight( bool forwardLightingEnabled,
                            bool defferedLightingEnabled,
                            bool infinityAreaMode,
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
    addChildProtected(*_forwardApplicator);
  }

  if (defferedLightingEnabled)
  {
    _defferedApplicator.reset(new AmbientLightApplicator(
                                                      _lightData,
                                                      _ambientMapSampler,
                                                      _diffuseLuminanceSampler,
                                                      device));
    addChildProtected(*_defferedApplicator);
  }

  _lightData.illuminance = glm::vec3(1.f);
  _lightData.distance = 100.f;
  _lightData.saturationDistance = 0.f;
  _lightData.infinityAreaMode = infinityAreaMode;

  _updateBound();
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

size_t AmbientLight::culledDrawablesNumber() const noexcept
{
  if (_lightData.infinityAreaMode) return 0;
  else return _defferedApplicator == nullptr ? 0 : 1;
}

DrawableNode& AmbientLight::culledDrawable(size_t index) noexcept
{
  if (_lightData.infinityAreaMode || _defferedApplicator == nullptr) Abort("AmbientLight::culledDrawable: no culled drawables available.");
  else return *_defferedApplicator;
}

size_t AmbientLight::areaModificatorsNumber() const noexcept
{
  return _forwardApplicator == nullptr ? 0 : 1;
}

AreaModificator& AmbientLight::areaModificator(size_t index) noexcept
{
  if(_forwardApplicator == nullptr) Abort("AmbientLight::areaModificator: no area modificators available.");
  else return *_forwardApplicator;
}

size_t AmbientLight::unculledDrawablesNumber() const noexcept
{
  if (_lightData.infinityAreaMode)
  {
    return _defferedApplicator == nullptr ? 0 : 1;
  }
  else return 0;
}

Drawable& AmbientLight::unculledDrawable(size_t index) noexcept
{
  if (!_lightData.infinityAreaMode || _defferedApplicator == nullptr) Abort("AmbientLight::unculledDrawable: no unculled drawables available.");
  else return *_defferedApplicator;
}
