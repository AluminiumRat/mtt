#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace mtt
{
  namespace clPipeline
  {
    struct SpotLightData
    {
      alignas(16) glm::vec3 illuminance;
      alignas(16) glm::vec3 lightPosition;
      alignas(4) float distance;
      alignas(4) float halfangleTan;
      alignas(16) glm::mat4 clipToView;
      alignas(16) glm::mat4 viewToLocal;
      alignas(4) float blurRadius;
      alignas(16) glm::mat4 localToShadowCoords;
    };
  }
}