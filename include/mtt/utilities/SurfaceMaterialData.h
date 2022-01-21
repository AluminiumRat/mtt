#pragma once

#include <glm/vec3.hpp>

namespace mtt
{
  struct SurfaceMaterialData
  {
    alignas(16) glm::vec3 albedo = glm::vec3(1);
    alignas(4) float roughness = .5;
    alignas(4) float specularStrength = 0;
    alignas(4) float metallic = 0;
    alignas(4) float reflectionFactor = 0;
    alignas(16) glm::vec3 emission = glm::vec3(0);
    alignas(4) float opaque = 1;
  };
}