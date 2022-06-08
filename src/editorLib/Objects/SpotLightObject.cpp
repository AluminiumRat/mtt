#include <mtt/editorLib/Objects/SpotLightObject.h>

using namespace mtt;

SpotLightObject::SpotLightObject( const QString& name,
                                  bool canBeRenamed,
                                  const UID theId) :
  LightObject(name, canBeRenamed, theId),
  _angle(glm::pi<float>() / 4.f),
  _shadowsEnabled(false),
  _shadowmapSize(256),
  _blurAngle(0.f)
{
}

void SpotLightObject::setAngle(float newValue) noexcept
{
  newValue = glm::clamp(newValue, 0.f, glm::pi<float>() / 2.f);
  if(_angle == newValue) return;
  _angle = newValue;
  emit angleChanged(_angle);
}

void SpotLightObject::setFilterImage(const QString& newValue) noexcept
{
  if (_filterImage == newValue) return;
  _filterImage = newValue;
  emit filterImageChanged(newValue);
}

void SpotLightObject::setShadowsEnabled(bool newValue) noexcept
{
  if (_shadowsEnabled == newValue) return;
  _shadowsEnabled = newValue;
  emit shadowsEnabledChanged(_shadowsEnabled);
}

void SpotLightObject::setShadowmapSize(size_t newValue) noexcept
{
  if (_shadowmapSize == newValue) return;
  _shadowmapSize = newValue;
  emit shadowmapSizeChanged(_shadowmapSize);
}

void SpotLightObject::setBlurAngle(float newValue) noexcept
{
  newValue = std::max(0.f, newValue);
  if(_blurAngle == newValue) return;
  _blurAngle = newValue;
  emit blurAngleChanged(_blurAngle);
}
