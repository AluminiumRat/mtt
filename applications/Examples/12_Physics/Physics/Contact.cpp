#include <glm/glm.hpp>

#include <Physics/Contact.h>
#include <Physics/PhysicsBoxObject.h>

Contact::Contact( PhysicsObject& object1,
                  PhysicsObject& object2,
                  const Collision& collision) :
  _object1(object1),
  _object2(object2),
  _collision(collision),
  _bounceSpeed(0)
{
  glm::vec2 object1Speed = object1.pointSpeed(_collision.position);
  glm::vec2 object2Speed = object2.pointSpeed(_collision.position);
  float penetrationSpeed = glm::dot(object1Speed, _collision.normal) -
                                      glm::dot(object2Speed, _collision.normal);
  penetrationSpeed = glm::max(0.f, penetrationSpeed);
  _bounceSpeed = bounceFactor * penetrationSpeed;

  _probingImpulseValue = 2.f * penetrationSpeed /
                                        (object1.invMass() + object2.invMass());

  Impulse probingImpulse{};
  probingImpulse._targetPoint = _collision.position;
  probingImpulse._impulseValue = -_collision.normal * _probingImpulseValue;
  glm::vec2 changedObject1Speed = object1.estimateChangedPointSpeed(
                                                            _collision.position,
                                                            probingImpulse);

  Impulse invProbingImpulse{};
  invProbingImpulse._targetPoint = _collision.position;
  invProbingImpulse._impulseValue = _collision.normal * _probingImpulseValue;
  glm::vec2 changedObject2Speed = object2.estimateChangedPointSpeed(
                                                            _collision.position,
                                                            invProbingImpulse);

  float changedPenetrationSpeed =
                          glm::dot(changedObject1Speed, _collision.normal) -
                              glm::dot(changedObject2Speed, _collision.normal);
  _penetrationSpeedChanging = changedPenetrationSpeed - penetrationSpeed;
}

void Contact::resolve() const noexcept
{
  if(_penetrationSpeedChanging == 0.f) return;

  glm::vec2 object1Speed = _object1.pointSpeed(_collision.position);
  glm::vec2 object2Speed = _object2.pointSpeed(_collision.position);
  float penetrationSpeed = glm::dot(object1Speed, _collision.normal) -
                                      glm::dot(object2Speed, _collision.normal);
  float impulseMultipler =
                (-_bounceSpeed - penetrationSpeed) / _penetrationSpeedChanging;

  Impulse realImpulse{};
  realImpulse._targetPoint = _collision.position;
  realImpulse._impulseValue =
                  impulseMultipler * _probingImpulseValue * -_collision.normal;
  _object1.applyImpulse(realImpulse);

  realImpulse._impulseValue = -realImpulse._impulseValue;
  _object2.applyImpulse(realImpulse);
}
