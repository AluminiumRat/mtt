#pragma once

#include <glm/vec2.hpp>

/// An force impulse that is applied to concrete point
struct Impulse
{
  glm::vec2 _impulseValue;
  glm::vec2 _targetPoint;
};