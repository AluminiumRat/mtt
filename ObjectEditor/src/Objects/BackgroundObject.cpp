#include <Objects/BackgroundObject.h>
#include <Objects/CubemapObject.h>

BackgroundObject::BackgroundObject(const mtt::UID& theId) :
  RotatableObject(theId),
  _lightEnabled(false),
  _luminance(1.f),
  _color(1.f),
  _dissolutionStartDistance(40.f),
  _dissolutionLength(10.f),
  _cubemap(nullptr)
{
  mtt::UID cubemapId(id().mixedUID(15285932983551776433ull));
  std::unique_ptr<CubemapObject> cubemap(new CubemapObject(cubemapId));
  _cubemap = cubemap.get();
  _cubemap->setObjectName(tr("Cubemap"));
  addSubobject(std::move(cubemap));
}

void BackgroundObject::setLightEnabled(bool newValue) noexcept
{
  if(_lightEnabled == newValue) return;
  _lightEnabled = newValue;
  emit lightEnabledChanged(newValue);
}

void BackgroundObject::setLuminance(float newValue) noexcept
{
  if (_luminance == newValue) return;
  _luminance = newValue;
  emit luminanceChanged(newValue);
}

void BackgroundObject::setColor(const glm::vec3& newValue) noexcept
{
  if(_color == newValue) return;
  _color = newValue;
  emit colorChanged(newValue);
}

void BackgroundObject::setDissolutionStartDistance(float newValue) noexcept
{
  if (_dissolutionStartDistance == newValue) return;
  _dissolutionStartDistance = newValue;
  emit dissolutionStartDistanceChanged(newValue);
}

void BackgroundObject::setDissolutionLength(float newValue) noexcept
{
  if (_dissolutionLength == newValue) return;
  _dissolutionLength = newValue;
  emit dissolutionLengthChanged(newValue);
}
