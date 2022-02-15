#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Objects/CubemapObject.h>

using namespace mtt;

BackgroundObject::BackgroundObject( const QString& name,
                                    bool canBeRenamed,
                                    const UID& theId) :
  RotatableObject(name, canBeRenamed, theId),
  _lightEnabled(false),
  _luminance(1.f),
  _color(1.f),
  _dissolutionStartDistance(40.f),
  _dissolutionLength(10.f),
  _cubemap(nullptr)
{
  UID cubemapId(id().mixedUID(15285932983551776433ull));
  std::unique_ptr<CubemapObject> cubemap(new CubemapObject( tr("Cubemap"),
                                                            false,
                                                            cubemapId));
  _cubemap = cubemap.get();
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
