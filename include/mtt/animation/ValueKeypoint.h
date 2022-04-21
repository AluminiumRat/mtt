#pragma once

#include <mtt/animation/Interpolator.h>
#include <mtt/animation/KeypointsAnimatedValue.h>

namespace mtt
{
  template <typename ValueType, typename TimeType>
  class ValueKeypoint
  {
  public:
    using ValueT = ValueType;
    using TimeT = TimeType;
    using InterpolatorT = Interpolator<ValueKeypoint>;
    using OwnerT = KeypointsAnimatedValue<ValueKeypoint>;

  public:
    inline ValueKeypoint() noexcept;
    inline ValueKeypoint( const ValueT& value,
                          TimeT time,
                          InterpolationType interpolation) noexcept;
    ValueKeypoint(const ValueKeypoint& other) noexcept;
    ValueKeypoint& operator = (const ValueKeypoint& other);
    virtual ~ValueKeypoint() noexcept = default;
  
    inline TimeType time() const noexcept;
    inline void setTime(TimeType newValue);

    inline const ValueType& value() const noexcept;
    inline void setValue(const ValueType& newValue);
  
    inline InterpolationType interpolation() const noexcept;
    inline void setInterpolation(InterpolationType newValue);

  private:
    friend KeypointsAnimatedValue<ValueKeypoint>;
    inline void setOwnerPtr(OwnerT* newOwner) noexcept;

  private:
    TimeType _time;
    ValueType _value;
    InterpolationType _interpolation;
    OwnerT* _owner;
  };

  template <typename ValueType, typename TimeType>
  inline ValueKeypoint<ValueType, TimeType>::ValueKeypoint() noexcept:
    _time(0),
    _interpolation(STEP_INTERPOLATION),
    _owner(nullptr)
  {
  }

  template <typename ValueType, typename TimeType>
  inline ValueKeypoint<ValueType, TimeType>::ValueKeypoint(
                                    const ValueType& value,
                                    TimeType time,
                                    InterpolationType interpolation) noexcept:
    _time(time),
    _value(value),
    _interpolation(interpolation),
    _owner(nullptr)
  {
  }

  template <typename ValueType, typename TimeType>
  ValueKeypoint<ValueType, TimeType>::ValueKeypoint(
                                          const ValueKeypoint& other) noexcept :
    _time(other._time),
    _value(other._value),
    _interpolation(other._interpolation),
    _owner(nullptr)
  {
  }

  template <typename ValueType, typename TimeType>
  ValueKeypoint<ValueType, TimeType>&
    ValueKeypoint<ValueType, TimeType>::operator = (const ValueKeypoint& other)
  {
    setTime(other.time());
    setValue(other.value());
    setInterpolation(other.interpolation());
  }

  template <typename ValueType, typename TimeType>
  inline TimeType ValueKeypoint<ValueType, TimeType>::time() const noexcept
  {
    return _time;
  }

  template <typename ValueType, typename TimeType>
  inline void ValueKeypoint<ValueType, TimeType>::setTime(TimeType newValue)
  {
    if(_time == newValue) return;

    TimeType oldTime = _time;
    _time = newValue;

    try
    {
      if(_owner != nullptr) _owner->sortKeypoints();
    }
    catch(...)
    {
      _time = oldTime;
      throw;
    }
  }

  template <typename ValueType, typename TimeType>
  inline const ValueType&
                      ValueKeypoint<ValueType, TimeType>::value() const noexcept
  {
    return _value;
  }

  template <typename ValueType, typename TimeType>
  inline void ValueKeypoint<ValueType, TimeType>::setValue(
                                                      const ValueType& newValue)
  {
    _value = newValue;
  }

  template <typename ValueType, typename TimeType>
  inline InterpolationType
              ValueKeypoint<ValueType, TimeType>::interpolation() const noexcept
  {
    return _interpolation;
  }

  template <typename ValueType, typename TimeType>
  inline void ValueKeypoint<ValueType, TimeType>::setInterpolation(
                                                    InterpolationType newValue)
  {
    _interpolation = newValue;
  }

  template <typename ValueType, typename TimeType>
  inline void ValueKeypoint<ValueType, TimeType>::setOwnerPtr(
          KeypointsAnimatedValue<ValueKeypoint<ValueType, TimeType>>* newOwner)
                                                                        noexcept
  {
    _owner = newOwner;
  }
}