#include <mtt/editorLib/Objects/DirectLightObject.h>

using namespace mtt;

DirectLightObject::DirectLightObject( const QString& name,
                                      bool canBeRenamed,
                                      const UID theId) :
  LightObject(name, canBeRenamed, theId),
  _radius(10),
  _shadowsEnabled(false),
  _shadowmapSize(256),
  _cascadeSize(1),
  _blurSize(0.f)
{
}

void DirectLightObject::setRadius(float newValue) noexcept
{
  if(_radius == newValue) return;
  _radius = newValue;
  emit radiusChanged(_radius);
}

void DirectLightObject::setShadowsEnabled(bool newValue) noexcept
{
  if (_shadowsEnabled == newValue) return;
  _shadowsEnabled = newValue;
  emit shadowsEnabledChanged(_shadowsEnabled);
}

void DirectLightObject::setShadowmapSize(size_t newValue) noexcept
{
  if (_shadowmapSize == newValue) return;
  _shadowmapSize = newValue;
  emit shadowmapSizeChanged(_shadowmapSize);
}

void DirectLightObject::setCascadeSize(size_t newValue) noexcept
{
  if (_cascadeSize == newValue) return;
  _cascadeSize = newValue;
  emit cascadeSizeChanged(_cascadeSize);
}

void DirectLightObject::setBlurSize(float newValue) noexcept
{
  if(_blurSize == newValue) return;
  _blurSize = newValue;
  emit blurSizeChanged(_blurSize);
}
