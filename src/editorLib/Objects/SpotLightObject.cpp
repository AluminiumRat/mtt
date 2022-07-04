#include <mtt/editorLib/Objects/SpotLightObject.h>

using namespace mtt;

SpotLightObject::SpotLightObject( const QString& name,
                                  bool canBeRenamed,
                                  const UID theId) :
  FadingLightObject(name, canBeRenamed, theId),
  _angle(glm::pi<float>() / 4.f)
{
  setDistance(5.f);
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
