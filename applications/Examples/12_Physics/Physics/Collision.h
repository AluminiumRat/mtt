#pragma once

#include <glm/vec2.hpp>

/// Contact between 2 objects, that leads to apply force
struct Collision
{
  glm::vec2 position;       // Point of application of force
  glm::vec2 normal;         // The direction of the contact force from first
                            // object to second
  float penetration;        // Depth of objects penetration
};