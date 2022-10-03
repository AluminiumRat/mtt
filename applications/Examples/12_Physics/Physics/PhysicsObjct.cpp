#include <mtt/utilities/Abort.h>

#include <Physics/PhysicsObject.h>

PhysicsObject::PhysicsObject(ObjectType objectType) :
  _objectType(objectType),
  _position(0.f),
  _speed(0.f),
  _rotation(0.f),
  _rotationSpeed(0.f),
  _invMass(0.f),
  _invMomentOfInertia(0.f)
{
}

void PhysicsObject::applyTime(float dTime) noexcept
{
  setPosition(position() + _speed * dTime);
  setRotation(rotation() + _rotationSpeed * dTime);
}

void PhysicsObject::_applyImpulse(glm::vec2& theSpeed,
                                  float& theRotationSpeed,
                                  const Impulse& impulse) const noexcept
{
  theSpeed += impulse._impulseValue * _invMass;

  glm::vec2 radiusVector = impulse._targetPoint - _position;
  float cross = radiusVector.x * impulse._impulseValue.y -
                                    radiusVector.y * impulse._impulseValue.x;
  theRotationSpeed += cross * _invMomentOfInertia;
}

glm::vec2 PhysicsObject::_pointSpeed( glm::vec2 pointPosition,
                                      const glm::vec2& theSpeed,
                                      float theRotationSpeed) const noexcept
{
  glm::vec2 toPoint = pointPosition - position();
  glm::vec2 tangent(-toPoint.y, toPoint.x);
  return theSpeed + tangent * theRotationSpeed;
}

glm::vec2 PhysicsObject::estimateChangedPointSpeed(
                                glm::vec2 pointPosition,
                                const Impulse& additionalImpulse) const noexcept
{
  glm::vec2 changedSpeed = speed();
  float changedRotationSpeed = rotationSpeed();
  _applyImpulse(changedSpeed, changedRotationSpeed, additionalImpulse);
  return _pointSpeed(pointPosition, changedSpeed, changedRotationSpeed);
}

void PhysicsObject::setPosition(glm::vec2 newValue) noexcept
{
  _position = newValue;
}

void PhysicsObject::setSpeed(glm::vec2 newValue) noexcept
{
  _speed = newValue;
}

void PhysicsObject::setRotation(float newValue) noexcept
{
  _rotation = newValue;
}

void PhysicsObject::setRotationSpeed(float newValue) noexcept
{
  _rotationSpeed = newValue;
}

void PhysicsObject::setInvMass(float newValue) noexcept
{
  _invMass = std::max(newValue, 0.f);
}

void PhysicsObject::setInvMomentOfInertia(float newValue) noexcept
{
  _invMomentOfInertia = std::max(newValue, 0.f);
}
