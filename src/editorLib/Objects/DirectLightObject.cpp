#include <mtt/editorLib/Objects/DirectLightObject.h>

using namespace mtt;

DirectLightObject::DirectLightObject( const QString& name,
                                      bool canBeRenamed,
                                      const UID theId) :
  LightWithShadowsObject(name, canBeRenamed, theId),
  _radius(10),
  _cascadeSize(1)
{
}

void DirectLightObject::setRadius(float newValue) noexcept
{
  newValue = glm::max(newValue, 0.f);
  if(_radius == newValue) return;
  _radius = newValue;
  emit radiusChanged(_radius);
}

void DirectLightObject::setCascadeSize(size_t newValue) noexcept
{
  if (_cascadeSize == newValue) return;
  _cascadeSize = newValue;
  emit cascadeSizeChanged(_cascadeSize);
}
