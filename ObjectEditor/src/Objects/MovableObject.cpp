#include <glm/gtc/matrix_transform.hpp>

#include <Objects/MovableObject.h>

MovableObject::MovableObject(const mtt::UID& id) :
  RotatableObject(id)
{
}

void MovableObject::setPosition(const glm::vec3& newValue) noexcept
{
  if (_position == newValue) return;
  _position = newValue;

  updateLocalTransform();
  
  emit positionChanged(_position);
}

void MovableObject::updateLocalTransform() noexcept
{
  glm::mat4 transform = glm::translate(glm::mat4(1), _position) *
                                                    glm::mat4_cast(rotation());
  setLocalTransform(transform);
}
