#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace mtt
{
  namespace clPipeline
  {
    struct DirectLightData
    {
      alignas(16) glm::vec3 illuminance;
      alignas(16) glm::vec3 lightInverseDirection;
      alignas(4) float height;
      alignas(4) float shadowDistance;
      alignas(16) glm::mat4 clipToView;
      alignas(16) glm::mat4 viewToLocal;
      alignas(16) glm::mat4 localToShadowCoords;
    };
  }
}