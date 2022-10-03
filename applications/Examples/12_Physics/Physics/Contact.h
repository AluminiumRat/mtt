#pragma once

#include <Physics/Collision.h>
#include <Physics/Impulse.h>

class PhysicsObject;

class Contact
{
public:
  static constexpr float bounceFactor = 0.9f;

public:
  Contact(PhysicsObject& object1,
          PhysicsObject& object2,
          const Collision& collision);
  Contact(const Contact&) noexcept = default;
  Contact& operator = (const Contact&) noexcept = default;
  ~Contact() noexcept = default;

  void resolve() const noexcept;

private:
  PhysicsObject& _object1;
  PhysicsObject& _object2;
  Collision _collision;
  float _bounceSpeed;
  float _probingImpulseValue;
  float _penetrationSpeedChanging;
};