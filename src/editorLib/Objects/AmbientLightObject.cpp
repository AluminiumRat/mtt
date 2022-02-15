#include <mtt/editorLib/Objects/AmbientLightObject.h>
#include <mtt/editorLib/Objects/CubemapObject.h>

using namespace mtt;

AmbientLightObject::AmbientLightObject( const QString& name,
                                        bool canBeRenamed,
                                        const UID theId) :
  LightObject(name, canBeRenamed, theId),
  _saturationDistance(0),
  _ambientMap(nullptr)
{
  UID ambientMapId(id().mixedUID(5272488623647734950ull));
  std::unique_ptr<CubemapObject> ambientMap(new CubemapObject(tr("Ambient map"),
                                                              false,
                                                              ambientMapId));
  _ambientMap = ambientMap.get();
  addSubobject(std::move(ambientMap));
}

void AmbientLightObject::setSaturationDistance(float newValue) noexcept
{
  if(_saturationDistance == newValue) return;
  _saturationDistance = newValue;
  emit saturationDistanceChanged(_saturationDistance);
}
