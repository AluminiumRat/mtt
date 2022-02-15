#include <mtt/editorLib/Objects/RotatableObject.h>

using namespace mtt;

RotatableObject::RotatableObject( const QString& name,
                                  bool canBeRenamed,
                                  const UID& id) :
  Object3D(name, canBeRenamed, id),
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
