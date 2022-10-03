#include <limits>

#include <glm/gtx/norm.hpp>
#include <glm/glm.hpp>

#include <Physics/PhysicsBoxObject.h>
#include <Physics/PhysicsCircleObject.h>

PhysicsBoxObject::PhysicsBoxObject() :
  PhysicsObject(BOX),
  _size(1.f),
  _sinRotation(0.f),
  _cosRotation(1.f)
{
  _updateGeometry();
}

void PhysicsBoxObject::setSize(glm::vec2 newValue) noexcept
{
  _size = glm::max(newValue, glm::vec2(0.f));
  _updateGeometry();
}

void PhysicsBoxObject::setRotation(float newValue) noexcept
{
  PhysicsObject::setRotation(newValue);
  _updateGeometry();
}

void PhysicsBoxObject::setPosition(glm::vec2 newValue) noexcept
{
  PhysicsObject::setPosition(newValue);
  _updateGeometry();
}

void PhysicsBoxObject::_updateGeometry() noexcept
{
  _sinRotation = sin(rotation());
  _cosRotation = cos(rotation());

  glm::vec2 halfSize = size() / 2.f;

  _localVertices[0] = glm::vec2(-halfSize.x, -halfSize.y);
  _worldVertices[0] = _pointToWorld(_localVertices[0]);
  _localNormals[0] = glm::vec2(-1.f, 0.f);
  _worldNormals[0] = _vectorToWorld(_localNormals[0]);

  _localVertices[1] = glm::vec2(-halfSize.x, halfSize.y);
  _worldVertices[1] = _pointToWorld(_localVertices[1]);
  _localNormals[1] = glm::vec2(0.f, 1.f);
  _worldNormals[1] = _vectorToWorld(_localNormals[1]);

  _localVertices[2] = glm::vec2(halfSize.x, halfSize.y);
  _worldVertices[2] = _pointToWorld(_localVertices[2]);
  _localNormals[2] = glm::vec2(1.f, 0.f);
  _worldNormals[2] = _vectorToWorld(_localNormals[2]);

  _localVertices[3] = glm::vec2(halfSize.x, -halfSize.y);
  _worldVertices[3] = _pointToWorld(_localVertices[3]);
  _localNormals[3] = glm::vec2(0.f, -1.f);
  _worldNormals[3] = _vectorToWorld(_localNormals[3]);
}

glm::vec2 PhysicsBoxObject::_pointToLocal(glm::vec2 point) const noexcept
{
  glm::vec2 localPoint = point - position();
  localPoint = glm::vec2(
                    localPoint.x * _cosRotation + localPoint.y * _sinRotation,
                    -localPoint.x * _sinRotation + localPoint.y * _cosRotation);
  return localPoint;
}

glm::vec2 PhysicsBoxObject::_vectorToLocal(glm::vec2 vector) const noexcept
{
  return glm::vec2( vector.x * _cosRotation + vector.y * _sinRotation,
                    -vector.x * _sinRotation + vector.y * _cosRotation);
}

glm::vec2 PhysicsBoxObject::_pointToWorld(glm::vec2 point) const noexcept
{
  glm::vec2 worldPoint( point.x * _cosRotation - point.y * _sinRotation,
                        point.x * _sinRotation + point.y * _cosRotation);
  worldPoint += position();
  return worldPoint;
}

glm::vec2 PhysicsBoxObject::_vectorToWorld(glm::vec2 vector) const noexcept
{
  return glm::vec2( vector.x * _cosRotation - vector.y * _sinRotation,
                    vector.x * _sinRotation + vector.y * _cosRotation);
}

std::optional<Collision> PhysicsBoxObject::_collisionToWorld(
                              std::optional<Collision> collision) const noexcept
{
  if (collision.has_value())
  {
    collision->position = _pointToWorld(collision->position);
    collision->normal = _vectorToWorld(collision->normal);
  }
  return collision;
}

std::optional<Collision> PhysicsBoxObject::intersect(
                                    const PhysicsObject& other) const noexcept
{
  if (other.objectType() == CIRCLE)
  {
    float radius = static_cast<const PhysicsCircleObject&>(other).radius();
    return _intersectWorldCircle(other.position(), radius);
  }

  if (other.objectType() == BOX)
  {
    // The basic idea of this intersection algorithm is derived from
    // https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-oriented-rigid-bodies--gamedev-8032
    // The _intersectBox method provides only half information about
    // possible intersections. Thus we gets information twice: from this box and
    // from another
    const PhysicsBoxObject& otherBox =
                                    static_cast<const PhysicsBoxObject&>(other);
    BoxIntersection forwardIntersection = _intersectBox(otherBox);
    BoxIntersection backIntersection = otherBox._intersectBox(*this);
    if (backIntersection.collision.has_value())
    {
      backIntersection.collision->normal = -backIntersection.collision->normal;
    }

    if (backIntersection.penetration < forwardIntersection.penetration)
    {
      return backIntersection.collision;
    }
    else return forwardIntersection.collision;
  }

  return std::optional<Collision>();
}

std::optional<Collision> PhysicsBoxObject::_intersectWorldCircle(
                                                glm::vec2 circleCenter,
                                                float radius) const noexcept
{
  glm::vec2 localCenter = _pointToLocal(circleCenter);
  std::optional<Collision> collision = _intersectLocalCircle( localCenter,
                                                              radius);
  return _collisionToWorld(collision);
}

std::optional<Collision> PhysicsBoxObject::_intersectLocalCircle(
                                                    glm::vec2 circleCenter,
                                                    float radius) const noexcept
{
  // The base ide of this algorithm is derived from
  // https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331

  if(circleCenter == glm::vec2(0.f)) return std::optional<Collision>();

  glm::vec2 halfSize = _size / 2.f;
  // Closest to circle point of the box
  glm::vec2 closest = glm::clamp(circleCenter, -halfSize, halfSize);

  // The circle center is inside the box
  if (closest == circleCenter)
  {
    Collision collision;
    collision.position = circleCenter;
    collision.penetration = std::numeric_limits<float>::max();

    for (size_t sideIndex = 0; sideIndex < 4; sideIndex++)
    {
      float toSurface = glm::dot( _localNormals[sideIndex],
                                  _localVertices[sideIndex] - circleCenter);
      if (toSurface < collision.penetration)
      {
        collision.penetration = toSurface;
        collision.normal = _localNormals[sideIndex];
      }
    }

    collision.penetration += radius;
    return collision;
  }

  glm::vec2 closestToCircleCenter = circleCenter - closest;
  float distanceToCenter = glm::length(closestToCircleCenter);
  if(distanceToCenter > radius) return std::optional<Collision>();

  Collision collision;
  collision.position = closest;
  collision.normal = glm::normalize(closestToCircleCenter);
  collision.penetration = radius - distanceToCenter;
  return collision;
}

PhysicsBoxObject::BoxIntersection PhysicsBoxObject::_intersectBox(
                                  const PhysicsBoxObject& other) const noexcept
{
  // The basic idea of this intersection algorithm is derived from
  // https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-oriented-rigid-bodies--gamedev-8032

  // Finding the box side with the minimum penetration distance
  size_t bestSideIndex = 0;
  float bestSidePenetration = other._getPlanePenetration( _worldVertices[0],
                                                          _worldNormals[0]);
  for (size_t sideIndex = 1; sideIndex < 4; sideIndex++)
  {
    float currentSidePenetration =
                          other._getPlanePenetration( _worldVertices[sideIndex],
                                                      _worldNormals[sideIndex]);
    if (currentSidePenetration < bestSidePenetration)
    {
      bestSideIndex = sideIndex;
      bestSidePenetration = currentSidePenetration;
    }
  }
  BoxIntersection intersection{};
  intersection.penetration = bestSidePenetration;
  // No contact points, returns only penetration depth information
  if(bestSidePenetration < 0.f) return intersection;

  // Calculation of the position of the application of force
  int verticesInsideCount = 0;
  glm::vec2 positionAccumulator(0.f);
  for (size_t vertexIndex = 0; vertexIndex < 4; vertexIndex++)
  {
    glm::vec2 vertex = other._worldVertices[vertexIndex];
    if (_isPointInside(vertex))
    {
      positionAccumulator += vertex;
      verticesInsideCount++;
    }
  }
  if(verticesInsideCount == 0) return intersection;

  Collision collision{};
  collision.position = positionAccumulator / float(verticesInsideCount);
  collision.normal = _worldNormals[bestSideIndex];
  collision.penetration = bestSidePenetration;
  intersection.collision = collision;

  return intersection;
}

float PhysicsBoxObject::_getPlanePenetration(
                                          glm::vec2 planePoint,
                                          glm::vec2 planeNormal) const noexcept
{
  float penetration = dot(planePoint - _worldVertices[0], planeNormal);
  for (size_t vertexIndex = 1; vertexIndex < 4; vertexIndex++)
  {
    penetration = std::max(
                    penetration,
                    dot(planePoint - _worldVertices[vertexIndex], planeNormal));
  }
  return penetration;
}

bool PhysicsBoxObject::_isPointInside(glm::vec2 position) const noexcept
{
  for (size_t sideIndex = 0; sideIndex < 4; sideIndex++)
  {
    if(dot( position - _worldVertices[sideIndex],
            _worldNormals[sideIndex]) > 0.f)
    {
      return false;
    }
  }
  return true;
}
