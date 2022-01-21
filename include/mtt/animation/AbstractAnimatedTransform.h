#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/animation/AbstractAnimatedValue.h>

namespace mtt
{
  template <typename TimeType>
  using AbstractAnimatedTransform = AbstractAnimatedValue<glm::mat4, TimeType>;
};