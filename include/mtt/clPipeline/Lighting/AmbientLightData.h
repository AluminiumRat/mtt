#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace mtt
{
  namespace clPipeline
  {
    struct AmbientLightData
    {
      alignas(16) glm::vec3 illuminance;
      alignas(4) float distance;
      alignas(4) float saturationDistance;
      alignas(4) bool infinityAreaMode;
    };

    struct AmbientLightDrawData : public AmbientLightData
    {
      alignas(16) glm::vec3 position;
      alignas(16) glm::mat4 clipToView;
      alignas(16) glm::mat4 viewToLocal;
    };
  }
}