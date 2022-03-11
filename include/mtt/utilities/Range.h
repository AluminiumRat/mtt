#pragma once

#include <algorithm>

namespace mtt
{
  template<typename ValueType>
  class Range
  {
  public:
    inline Range() noexcept;
    inline Range(ValueType min, ValueType max) noexcept;
    Range(const Range&) noexcept = default;
    Range& operator = (const Range&) noexcept = default;
    ~Range() noexcept = default;

    inline ValueType min() const noexcept;
    inline ValueType max() const noexcept;

    inline void set(ValueType min, ValueType max) noexcept;

    inline bool operator == (const Range& other) const noexcept;

  private:
    ValueType _min;
    ValueType _max;
  };

  template<typename RangeValueType, typename MultiplerType>
  inline Range<RangeValueType> operator * ( MultiplerType multipler,
                                            const Range<RangeValueType>& range)
  {
    return Range<RangeValueType>( multipler * range.min(),
                                  multipler * range.max());
  }

  template<typename RangeValueType, typename MultiplerType>
  inline Range<RangeValueType> operator * ( const Range<RangeValueType>& range,
                                            MultiplerType multipler)
  {
    return Range<RangeValueType>( multipler * range.min(),
                                  multipler * range.max());
  }

  template<typename RangeValueType, typename DividerType>
  inline Range<RangeValueType> operator / ( const Range<RangeValueType>& range,
                                            DividerType divider)
  {
    return Range<RangeValueType>( range.min() / divider,
                                  range.max() / divider);
  }

  template<typename ValueType>
  inline Range<ValueType>::Range() noexcept :
    _min(0),
    _max(0)
  {
  }

  template<typename ValueType>
  inline Range<ValueType>::Range(ValueType min, ValueType max) noexcept :
    _min(0),
    _max(0)
  {
    set(min, max);
  }

  template<typename ValueType>
  inline ValueType Range<ValueType>::min() const noexcept
  {
    return _min;
  }

  template<typename ValueType>
  inline ValueType Range<ValueType>::max() const noexcept
  {
    return _max;
  }

  template<typename ValueType>
  inline void Range<ValueType>::set(ValueType min, ValueType max) noexcept
  {
    _min = std::min(min, max);
    _max = std::max(min, max);
  }

  template<typename ValueType>
  inline bool Range<ValueType>::operator == (const Range& other) const noexcept
  {
    return _min == other._min && _max == other._max;
  }
}