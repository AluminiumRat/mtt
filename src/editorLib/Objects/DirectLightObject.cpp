#include <mtt/editorLib/Objects/DirectLightObject.h>

using namespace mtt;

DirectLightObject::DirectLightObject( const QString& name,
                                      bool canBeRenamed,
                                      const UID theId) :
  LightWithShadowsObject(name, canBeRenamed, theId),
  _shadowDistance(50),
  _cascadeSize(1)
{
}

void DirectLightObject::setShadowDistance(float newValue) noexcept
{
  newValue = glm::max(newValue, 0.f);
  if(_shadowDistance == newValue) return;
  _shadowDistance = newValue;
  emit shadowDistanceChanged(_shadowDistance);
}

void DirectLightObject::setCascadeSize(size_t newValue) noexcept
{
  if (_cascadeSize == newValue) return;
  _cascadeSize = newValue;
  emit cascadeSizeChanged(_cascadeSize);
}
