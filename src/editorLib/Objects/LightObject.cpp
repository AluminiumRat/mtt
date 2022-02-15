#include <mtt/editorLib/Objects/LightObject.h>

using namespace mtt;

LightObject::LightObject( const QString& name,
                          bool canBeRenamed,
                          const UID theId) :
  EnvironmentObject(name, canBeRenamed, theId),
  _enabled(true),
  _distance(50),
  _color(1, 1, 1),
  _baseIlluminance(1)
{
}

void LightObject::setEnabled(bool newValue) noexcept
{
  if(_enabled == newValue) return;
  _enabled = newValue;
  emit enabledChanged(_enabled);
}

void LightObject::setDistance(float newValue) noexcept
{
  if(_distance == newValue) return;
  _distance = newValue;
  emit distanceChanged(_distance);
}

void LightObject::setColor(const glm::vec3& newValue) noexcept
{
  if(_color == newValue) return;
  _color = newValue;
  emit colorChanged(_color);
}

void LightObject::setBaseIlluminance(float newValue) noexcept
{
  if(_baseIlluminance == newValue) return;
  _baseIlluminance = newValue;
  emit baseIlluminanceChanged(_baseIlluminance);
}
