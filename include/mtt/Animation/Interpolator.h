#pragma once

#include <chrono>

#include <glm/common.hpp>

#include <mtt/Utilities/Abort.h>

namespace mtt
{
  enum InterpolationType
  {
    STEP_INTERPOLATION = 0,
    LINEAR_INTERPOLATION = 1
  };

  template <typename Rep, typename Period>
  inline float timeRatio( std::chrono::duration<Rep, Period> start,
                          std::chrono::duration<Rep, Period> finish,
                          std::chrono::duration<Rep, Period> middle);

  template <typename TimeType>
  inline float timeRatio(TimeType start, TimeType finish, TimeType middle);

  template <typename KeypointType>
  class Interpolator
  {
  public:
    using KeypointT = KeypointType;
    using ValueT = typename KeypointT::ValueT;
    using TimeT = typename KeypointT::TimeT;

    /// Warning!!! first.time should be smaller or equal that second.time and
    /// time argumetn should be between first.time and second.time.
    inline static ValueT calculate( const KeypointT& first,
                                    const KeypointT& second,
                                    TimeT time);
  };

  template <typename KeypointType>
  inline typename KeypointType::ValueT
                          Interpolator<KeypointType>::calculate(
                                            const KeypointType& first,
                                            const KeypointType& second,
                                            typename KeypointType::TimeT time)
  {
    TimeT startTime = first.time();
    TimeT finishTime = second.time();

    if(startTime > finishTime) Abort("Interpolator::calculate: startTime > finishTime");
    if(time < startTime) Abort("Interpolator::calculate: time < startTime");
    if(time > finishTime) Abort("Interpolator::calculate: time > finishTime");

    if(time == finishTime) return second.value();
    if(time == startTime || second.interpolation() == STEP_INTERPOLATION)
    {
      return first.value();
    }

    float ratio = timeRatio(startTime, finishTime, time);

    return glm::mix(first.value(), second.value(), ratio);
  }

  template <typename Rep, typename Period>
  inline float timeRatio( std::chrono::duration<Rep, Period> start,
                          std::chrono::duration<Rep, Period> finish,
                          std::chrono::duration<Rep, Period> middle)
  {
    return float(middle.count() - start.count()) /
                                          float(finish.count() - start.count());
  }

  template <typename TimeType>
  inline float timeRatio(TimeType start, TimeType finish, TimeType middle)
  {
    return float(middle - start) / float(finish - start);
  }
};