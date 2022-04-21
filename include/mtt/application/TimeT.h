#pragma once

#include <chrono>

#include <mtt/utilities/Range.h>

namespace mtt
{
  using TimeT = std::chrono::duration<int32_t, std::ratio<1, 1000>>;

  using TimeRange = Range<TimeT>;

  constexpr TimeT millisecond =
                std::chrono::duration_cast<TimeT>(std::chrono::milliseconds(1));
  constexpr TimeT second =
                    std::chrono::duration_cast<TimeT>(std::chrono::seconds(1));
  constexpr TimeT minute =
                    std::chrono::duration_cast<TimeT>(std::chrono::minutes(1));

  /// Returns seconds in float
  inline constexpr float toFloatTime(TimeT time) noexcept
  {
    return
        std::chrono::duration_cast<std::chrono::duration<float>>(time).count();
  }

  inline constexpr TimeT toTimeT(float timeInSeconds)
  {
    using FloatTimeType = std::chrono::duration<float>;
    FloatTimeType floatTime(timeInSeconds);
    return std::chrono::duration_cast<TimeT>(floatTime);
  }
}