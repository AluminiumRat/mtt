#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/Animation/AbstractAnimatedValue.h>

namespace mtt
{
  template <typename TimeType>
  using AbstractAnimatedTransform = AbstractAnimatedValue<glm::mat4, TimeType>;
};