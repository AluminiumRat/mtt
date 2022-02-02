#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace mtt
{
  namespace clPipeline
  {
    struct BackgroundProperties
    {
      alignas(16) glm::vec3 luminance;
      alignas(4) float dissolutionStartDistance;
      alignas(4) float dissolutionLength;
    };

    struct BackgroundDrawData
    {
      BackgroundProperties properties;
      alignas(16) glm::mat4 viewToLocal;
    };
  }
}