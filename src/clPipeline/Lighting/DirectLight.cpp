#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace clPipeline;

DirectLight::DirectLight( bool forwardLightingEnabled,
                          bool defferedLightingEnabled,
                          LogicalDevice& device) :
  _device(device),
  _shadowmapExtent(256),
  _shadowmapField(nullptr),
  _illuminance(1.f),
  _distance(50.f),
  _radius(10.f),
  _cascadeSize(1),
  _blurSize(0.f)
{
  addChildProtected(_shadowmapCamera);

  if(forwardLightingEnabled)
  {
    _forwardLightApplicator.reset(new DirectLightAreaModificator(*this));
    addChildProtected(*_forwardLightApplicator);
  }

  if(defferedLightingEnabled)
  {
    _defferedLightApplicator.reset(new DirectLightApplicator(*this, device));
    addChildProtected(*_defferedLightApplicator);
  }

  _updateBound();
}

void DirectLight::_updateShadowmapCamera() noexcept
{
  _shadowmapCamera.setOrthoProjection(-radius(),
                                      radius(),
                                      -radius(),
                                      radius(),
                                      0,
                                      distance());
}

void DirectLight::_updateBound() noexcept
{
  if (_defferedLightApplicator != nullptr)
  {
    _defferedLightApplicator->updateBound();
  }
  if(_forwardLightApplicator != nullptr)
  {
    _forwardLightApplicator->updateBound();
  }
}

void DirectLight::_resetPipelines() noexcept
{
  if (_defferedLightApplicator != nullptr)
  {
    _defferedLightApplicator->resetPipelines();
  }
  if (_forwardLightApplicator != nullptr) _forwardLightApplicator->reset();
}

void DirectLight::_resetShadowmapProvider() noexcept
{
  _resetPipelines();
  _shadowmapProvider.reset();
  _shadowmapSampler.reset();
}

void DirectLight::_updateShadowmapProvider()
{
  bool shadowsEnabled = _shadowmapExtent != 0 &&
                        _radius != 0 &&
                        _cascadeSize != 0 &&
                        _shadowmapField != nullptr;

  if (!shadowsEnabled)
  {
    if (_shadowmapProvider == nullptr) return;
    _resetShadowmapProvider();
  }
  else
  {
    if(_shadowmapProvider != nullptr) return;
    _resetPipelines();
    try
    {
      _shadowmapProvider.reset(new CascadeShadowMapProvider(
                                                  2,
                                                  glm::uvec2(_shadowmapExtent),
                                                  _device));
      _shadowmapProvider->setTargetField(_shadowmapField);

      _shadowmapSampler.reset(new Sampler(cascadeSize(),
                                          PipelineResource::VOLATILE,
                                          _device));

      for(size_t layerIndex = 0; layerIndex < cascadeSize(); layerIndex++)
      {
        _shadowmapSampler->setAttachedTexture(
                                          std::make_shared<Texture2D>(_device),
                                          layerIndex);
      }

      _shadowmapSampler->setMinFilter(VK_FILTER_LINEAR);
      _shadowmapSampler->setMagFilter(VK_FILTER_LINEAR);
      _shadowmapSampler->setMipmapMode(VK_SAMPLER_MIPMAP_MODE_NEAREST);
      _shadowmapSampler->setAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
      _shadowmapSampler->setAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
    }
    catch (...)
    {
      _resetShadowmapProvider();
      throw;
    }
  }
}

DirectLightDrawData DirectLight::buildDrawData(
                              const DrawPlanBuildInfo& buildInfo) const noexcept
{
  DirectLightDrawData drawData;
  drawData.illuminance = illuminance();
  drawData.lightInverseDirection =
              buildInfo.drawMatrices.localToViewMatrix * glm::vec4(0, 0, 1, 0);
  drawData.lightInverseDirection =
                                glm::normalize(drawData.lightInverseDirection);
  drawData.distance = distance();
  drawData.radius = radius();
  drawData.clipToView = buildInfo.drawMatrices.clipToViewMatrix;
  drawData.viewToLocal = glm::inverse(buildInfo.drawMatrices.localToViewMatrix);

  return drawData;
}

size_t DirectLight::culledDrawablesNumber() const noexcept
{
  return _defferedLightApplicator == nullptr ? 0 : 1;
}

DrawableNode& DirectLight::culledDrawable(size_t index) noexcept
{
  if (_defferedLightApplicator == nullptr) Abort("DirectLight::culledDrawable: no culled drawables available.");
  else return *_defferedLightApplicator;
}

size_t DirectLight::areaModificatorsNumber() const noexcept
{
  return _forwardLightApplicator == nullptr ? 0 : 1;
}

AreaModificator& DirectLight::areaModificator(size_t index) noexcept
{
  if (_forwardLightApplicator == nullptr) Abort("DirectLight::areaModificator: no area modificators available.");
  else return *_forwardLightApplicator;
}
