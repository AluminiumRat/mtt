#include <Objects/RotatableObject.h>

RotatableObject::RotatableObject(const mtt::UID& id) :
  Object3D(id),
  _rotation(1, 0, 0, 0)
{
}

void RotatableObject::setRotation(const glm::quat& newValue) noexcept
{
  if (_rotation == newValue) return;
  _rotation = newValue;

  updateLocalTransform();

  emit rotationChanged(_rotation);
}

void RotatableObject::updateLocalTransform() noexcept
{
  glm::mat4 transform = glm::mat4_cast(_rotation);
  setLocalTransform(transform);
}
