#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace mtt
{
  namespace clPipeline
  {
    struct PointLightData
    {
      alignas(16) glm::vec3 illuminance;
      alignas(16) glm::vec3 lightPosition;
      alignas(4) float distance;
      alignas(16) glm::mat4 clipToView;
      alignas(16) glm::mat4 viewToLocal;
      alignas(4) float blurRadius;
      alignas(4) uint32_t startSample;
      alignas(4) uint32_t endSample;
    };
  }
}