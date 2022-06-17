#include <mtt/editorLib/Objects/LightWithShadowsObject.h>

using namespace mtt;

LightWithShadowsObject::LightWithShadowsObject( const QString& name,
                                                bool canBeRenamed,
                                                const UID theId) :
  LightObject(name, canBeRenamed, theId),
  _shadowsEnabled(false),
  _shadowmapSize(256),
  _shadowBlur(0.f)
{
}

void LightWithShadowsObject::setShadowsEnabled(bool newValue) noexcept
{
  if (_shadowsEnabled == newValue) return;
  _shadowsEnabled = newValue;
  emit shadowsEnabledChanged(_shadowsEnabled);
}

void LightWithShadowsObject::setShadowmapSize(size_t newValue) noexcept
{
  if (_shadowmapSize == newValue) return;
  _shadowmapSize = newValue;
  emit shadowmapSizeChanged(_shadowmapSize);
}

void LightWithShadowsObject::setShadowBlur(float newValue) noexcept
{
  newValue = glm::max(newValue, 0.f);
  if(_shadowBlur == newValue) return;
  _shadowBlur = newValue;
  emit shadowBlurChanged(_shadowBlur);
}
