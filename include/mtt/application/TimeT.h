#pragma once

#include <chrono>

namespace mtt
{
  using TimeT = std::chrono::duration<int32_t, std::ratio<1, 1000>>;
}