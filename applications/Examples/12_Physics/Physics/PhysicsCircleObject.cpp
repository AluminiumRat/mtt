#include <glm/glm.hpp>

#include <Physics/PhysicsCircleObject.h>

PhysicsCircleObject::PhysicsCircleObject() :
  PhysicsObject(CIRCLE),
  _radius(1.f)
{
}

void PhysicsCircleObject::setRadius(float newValue) noexcept
{
  _radius = glm::max(newValue, 0.f);
}

std::optional<Collision> PhysicsCircleObject::intersect(
                                      const PhysicsObject& other) const noexcept
{
  std::optional<Collision> collision;
  if(position() == other.position()) return collision;

  if (other.objectType() == CIRCLE)
  {
    // Just checks the distance between the circle centers
    const PhysicsCircleObject& otherCircle =
                                static_cast<const PhysicsCircleObject&>(other);
    glm::vec2 toOther = other.position() - position();
    float distance = glm::length(toOther);
    distance = distance - radius() - otherCircle.radius();
    if (distance <= 0)
    {
      collision.emplace();
      collision->normal = glm::normalize(toOther);
      collision->position = position() +
                                collision->normal * (radius() + distance / 2.f);
      collision->penetration = -distance;
    }
  }
  else
  {
    collision = other.intersect(*this);
    if(collision.has_value()) collision->normal = -collision->normal;
  }

  return collision;
}
