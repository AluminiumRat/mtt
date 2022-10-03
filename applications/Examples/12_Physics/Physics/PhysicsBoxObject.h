#pragma once

#include <Physics/PhysicsObject.h>

class PhysicsBoxObject : public PhysicsObject
{
public:
  PhysicsBoxObject();
  PhysicsBoxObject(const PhysicsBoxObject&) = delete;
  const PhysicsBoxObject& operator = (const PhysicsBoxObject&) = delete;
  virtual ~PhysicsBoxObject() noexcept = default;

  inline glm::vec2 size() const noexcept;
  /// newValue must not be negative
  void setSize(glm::vec2 newValue) noexcept;

  virtual void setRotation(float newValue) noexcept override;
  virtual void setPosition(glm::vec2 newValue) noexcept override;

  virtual std::optional<Collision> intersect(
                          const PhysicsObject& other) const noexcept override;

private:
  /// Calculate the intersection with the circle.
  /// The circle center in world coordinates.
  std::optional<Collision> _intersectWorldCircle( glm::vec2 circleCenter,
                                                  float radius) const noexcept;
  /// Calculate the intersection with the circle.
  /// The circle center in local coordinates of this box.
  std::optional<Collision> _intersectLocalCircle( glm::vec2 circleCenter,
                                                  float radius) const noexcept;

  struct BoxIntersection
  {
    std::optional<Collision> collision;
    float penetration;
  };
  BoxIntersection _intersectBox(const PhysicsBoxObject& other) const noexcept;

  /// Calculates penetration depth of this box to the negative halfspace of the
  /// the plane.
  /// planePoint and planeNormal in world coordinates
  float _getPlanePenetration( glm::vec2 planePoint,
                              glm::vec2 planeNormal) const noexcept;
  /// Checks that the point is inside ot this box
  bool _isPointInside(glm::vec2 position) const noexcept;

  // Translates objects from the local space to the world space and back.
  glm::vec2 _pointToLocal(glm::vec2 point) const noexcept;
  glm::vec2 _vectorToLocal(glm::vec2 vector) const noexcept;
  glm::vec2 _pointToWorld(glm::vec2 point) const noexcept;
  glm::vec2 _vectorToWorld(glm::vec2 vector) const noexcept;
  std::optional<Collision> _collisionToWorld(
                            std::optional<Collision> collision) const noexcept;

  void _updateGeometry() noexcept;

private:
  glm::vec2 _size;

  // Subsidiary geometry information
  float _sinRotation;
  float _cosRotation;
  glm::vec2 _localVertices[4];
  glm::vec2 _worldVertices[4];
  glm::vec2 _localNormals[4];
  glm::vec2 _worldNormals[4];
};

inline glm::vec2 PhysicsBoxObject::size() const noexcept
{
  return _size;
}
