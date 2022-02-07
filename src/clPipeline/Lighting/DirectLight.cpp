#include <mtt/clPipeline/Lighting/DirectLight.h>

using namespace mtt;
using namespace clPipeline;

DirectLight::DirectLight(LogicalDevice& device) :
  _shadowMapProvider(nullptr),
  _illuminance(1.f),
  _distance(50.f),
  _radius(10.f),
  _cascadeSize(1),
  _blurSize(0.f),
  _defferedLightApplicator(*this, device)
{
  _updateBound();
}

DrawableNode* DirectLight::defferedLightApplicator() noexcept
{
  return &_defferedLightApplicator;
}

AreaModificator* DirectLight::forwardLightModificator() noexcept
{
  return nullptr;
}

void DirectLight::_updateBound() noexcept
{
  _defferedLightApplicator.updateBound();
}

void DirectLight::_resetPipelines() noexcept
{
  _defferedLightApplicator.resetPipelines();
}
