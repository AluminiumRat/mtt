#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/Texture2D.h>

using namespace mtt;
using namespace clPipeline;

DirectLight::DirectLight(LogicalDevice& device) :
  _device(device),
  _shadowMapProvider(nullptr),
  _illuminance(1.f),
  _distance(50.f),
  _radius(10.f),
  _cascadeSize(1),
  _blurSize(0.f),
  _defferedLightApplicator(*this, device),
  _forwardLightApplicator(*this)
{
  _updateBound();
}

DrawableNode* DirectLight::defferedLightApplicator() noexcept
{
  return &_defferedLightApplicator;
}

AreaModificator* DirectLight::forwardLightModificator() noexcept
{
  return &_forwardLightApplicator;
}

void DirectLight::_updateBound() noexcept
{
  _defferedLightApplicator.updateBound();
  _forwardLightApplicator.updateBound();
}

void DirectLight::_resetPipelines() noexcept
{
  _defferedLightApplicator.resetPipelines();
  _forwardLightApplicator.reset();
  _shadowmapSampler.reset();
}

Sampler& DirectLight::getOrCreateShdowmapSampler()
{
  if(_shadowmapSampler.has_value()) return *_shadowmapSampler;

  try
  {
    _shadowmapSampler.emplace(cascadeSize(),
                              PipelineResource::VOLATILE,
                              _device);

    for(size_t layerIndex = 0;
        layerIndex < cascadeSize();
        layerIndex++)
    {
      _shadowmapSampler->setAttachedTexture(
                                          std::make_shared<Texture2D>(_device),
                                          layerIndex);
    }

    _shadowmapSampler->setMinFilter(VK_FILTER_NEAREST);
    _shadowmapSampler->setMagFilter(VK_FILTER_NEAREST);
    _shadowmapSampler->setMipmapMode(VK_SAMPLER_MIPMAP_MODE_NEAREST);
    _shadowmapSampler->setAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
    _shadowmapSampler->setAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
  }
  catch(...)
  {
    _shadowmapSampler.reset();
    throw;
  }
  return *_shadowmapSampler;
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
