#pragma once

#include <Physics/PhysicsObject.h>

class PhysicsCircleObject : public PhysicsObject
{
public:
  PhysicsCircleObject();
  PhysicsCircleObject(const PhysicsCircleObject&) = delete;
  const PhysicsCircleObject& operator = (const PhysicsCircleObject&) = delete;
  virtual ~PhysicsCircleObject() noexcept = default;

  inline float radius() const noexcept;
  /// newValue must not be negative
  void setRadius(float newValue) noexcept;

  virtual std::optional<Collision> intersect(
                          const PhysicsObject& other) const noexcept override;

private:
  float _radius;
};

inline float PhysicsCircleObject::radius() const noexcept
{
  return _radius;
}
