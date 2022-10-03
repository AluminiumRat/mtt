#pragma once

#include <optional>

#include <glm/vec2.hpp>

#include <Physics/Collision.h>
#include <Physics/Impulse.h>

/// This is the base class for objects that model physical behaviour
class PhysicsObject
{
public:
  enum ObjectType
  {
    CIRCLE,   // PhysicsCircleObject
    BOX       // PhysicsBoxObject
  };

public:
  PhysicsObject(ObjectType objectType);
  PhysicsObject(const PhysicsObject&) = delete;
  PhysicsObject& operator = (const PhysicsObject&) = delete;
  virtual ~PhysicsObject() noexcept = default;

  inline ObjectType objectType() const noexcept;

  inline glm::vec2 position() const noexcept;
  virtual void setPosition(glm::vec2 newValue) noexcept;

  inline glm::vec2 speed() const noexcept;
  void setSpeed(glm::vec2 newValue) noexcept;

  inline float rotation() const noexcept;
  virtual void setRotation(float newValue) noexcept;

  inline float rotationSpeed() const noexcept;
  void setRotationSpeed(float newValue) noexcept;

  /// The reciprocal of the object's mass
  /// 0.f refers to infinity mass
  inline float invMass() const noexcept;
  /// newValue must not be negative
  void setInvMass(float newValue) noexcept;

  /// The reciprocal of the object's moment of inertia
  /// 0.f refers to infinity moment of inertia
  inline float invMomentOfInertia() const noexcept;
  /// newValue must not be negative
  void setInvMomentOfInertia(float newValue) noexcept;

  /// Update position and rotation based on speed and rotation speed
  void applyTime(float dTime) noexcept;
  /// Update speed and rotation speed based on force impulse
  inline void applyImpulse(const Impulse& impulse) noexcept;

  /// Calculate intersection with another object
  virtual std::optional<Collision> intersect(
                                const PhysicsObject& other) const noexcept = 0;

  /// Speed of object's fragment in a point.
  /// pointPosition in world space
  inline glm::vec2 pointSpeed(glm::vec2 pointPosition) const noexcept;
  /// Calculate how object's fragment speed will change if an impulse is
  /// applied to the object
  glm::vec2 estimateChangedPointSpeed(
                              glm::vec2 pointPosition,
                              const Impulse& additionalImpulse) const noexcept;

private:
  void _applyImpulse( glm::vec2& theSpeed,
                      float& theRotationSpeed,
                      const Impulse& impulse) const noexcept;
  glm::vec2 _pointSpeed(glm::vec2 pointPosition,
                        const glm::vec2& theSpeed,
                        float theRotationSpeed) const noexcept;

private:
  ObjectType _objectType;
  glm::vec2 _position;
  glm::vec2 _speed;
  float _rotation;
  float _rotationSpeed;
  float _invMass;
  float _invMomentOfInertia;
};

inline PhysicsObject::ObjectType PhysicsObject::objectType() const noexcept
{
  return _objectType;
}

inline glm::vec2 PhysicsObject::position() const noexcept
{
  return _position;
}

inline glm::vec2 PhysicsObject::speed() const noexcept
{
  return _speed;
}

inline float PhysicsObject::rotation() const noexcept
{
  return _rotation;
}

inline float PhysicsObject::rotationSpeed() const noexcept
{
  return _rotationSpeed;
}

inline float PhysicsObject::invMass() const noexcept
{
  return _invMass;
}

inline float PhysicsObject::invMomentOfInertia() const noexcept
{
  return _invMomentOfInertia;
}

inline void PhysicsObject::applyImpulse(const Impulse& impulse) noexcept
{
  _applyImpulse(_speed, _rotationSpeed, impulse);
}

inline glm::vec2 PhysicsObject::pointSpeed(
                                        glm::vec2 pointPosition) const noexcept
{
  return _pointSpeed(pointPosition, _speed, _rotationSpeed);
}
