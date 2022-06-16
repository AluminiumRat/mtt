#include <mtt/editorLib/Objects/PointLightObject.h>
#include <mtt/editorLib/Objects/CubemapObject.h>

using namespace mtt;

PointLightObject::PointLightObject( const QString& name,
                                    bool canBeRenamed,
                                    const UID theId) :
  LightObject(name, canBeRenamed, theId),
  _shadowsEnabled(false),
  _shadowmapSize(256),
  _blurAngle(0.f)
{
  UID cubemapId(id().mixedUID(15285932983551776433ull));
  std::unique_ptr<CubemapObject> filterCubemap(new CubemapObject( tr("Filter"),
                                                                  false,
                                                                  cubemapId));
  _filterCubemap = filterCubemap.get();
  addSubobject(std::move(filterCubemap));

  setDistance(5);
}

void PointLightObject::setShadowsEnabled(bool newValue) noexcept
{
  if (_shadowsEnabled == newValue) return;
  _shadowsEnabled = newValue;
  emit shadowsEnabledChanged(_shadowsEnabled);
}

void PointLightObject::setShadowmapSize(size_t newValue) noexcept
{
  if (_shadowmapSize == newValue) return;
  _shadowmapSize = newValue;
  emit shadowmapSizeChanged(_shadowmapSize);
}

void PointLightObject::setBlurAngle(float newValue) noexcept
{
  newValue = glm::clamp(newValue, 0.f, glm::pi<float>() / 10.f);
  if(_blurAngle == newValue) return;
  _blurAngle = newValue;
  emit blurAngleChanged(_blurAngle);
}
