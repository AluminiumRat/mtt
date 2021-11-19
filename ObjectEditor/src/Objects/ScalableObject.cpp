#include <glm/gtc/matrix_transform.hpp>

#include <Objects/ScalableObject.h>

ScalableObject::ScalableObject(const mtt::UID& id) :
  MovableObject(id),
  _scale(1.f)
{
}

void ScalableObject::setScale(const glm::vec3& newValue) noexcept
{
  if(_scale == newValue) return;
  _scale = newValue;

  updateLocalTransform();

  emit positionChanged(_scale);
}

void ScalableObject::updateLocalTransform() noexcept
{
  glm::mat4 transform = glm::translate(glm::mat4(1), position()) *
                        glm::mat4_cast(rotation()) *
                        glm::scale(glm::mat4(1), _scale);
  setLocalTransform(transform);
}
