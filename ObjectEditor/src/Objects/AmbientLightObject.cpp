#include <Objects/AmbientLightObject.h>
#include <Objects/CubemapObject.h>

AmbientLightObject::AmbientLightObject(const mtt::UID theId) :
  LightObject(theId),
  _saturationDistance(0),
  _ambientMap(nullptr)
{
  mtt::UID ambientMapId(id().mixedUID(5272488623647734950ull));
  std::unique_ptr<CubemapObject> ambientMap(new CubemapObject(ambientMapId));
  _ambientMap = ambientMap.get();
  _ambientMap->setObjectName(tr("Ambient map"));
  addSubobject(std::move(ambientMap));
}

void AmbientLightObject::setSaturationDistance(float newValue) noexcept
{
  if(_saturationDistance == newValue) return;
  _saturationDistance = newValue;
  emit saturationDistanceChanged(_saturationDistance);
}
