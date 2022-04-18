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

    inline bool isValid() const noexcept;

    inline ValueType min() const noexcept;
    inline ValueType start() const noexcept;

    inline ValueType max() const noexcept;
    inline ValueType finish() const noexcept;

    inline ValueType length() const noexcept;

    inline bool contains(ValueType value) const noexcept;

    inline void set(ValueType min, ValueType max) noexcept;

    inline void extend(ValueType value) noexcept;
    inline void extend(const Range<ValueType>& other) noexcept;

    inline bool operator == (const Range& other) const noexcept;

  private:
    bool _valid;
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
    _valid(false)
  {
  }

  template<typename ValueType>
  inline Range<ValueType>::Range(ValueType min, ValueType max) noexcept :
    _valid(true),
    _min(0),
    _max(0)
  {
    set(min, max);
  }

  template<typename ValueType>
  inline bool Range<ValueType>::isValid() const noexcept
  {
    return _valid;
  }

  template<typename ValueType>
  inline ValueType Range<ValueType>::min() const noexcept
  {
    return _min;
  }

  template<typename ValueType>
  inline ValueType Range<ValueType>::start() const noexcept
  {
    return min();
  }

  template<typename ValueType>
  inline ValueType Range<ValueType>::max() const noexcept
  {
    return _max;
  }

  template<typename ValueType>
  inline ValueType Range<ValueType>::finish() const noexcept
  {
    return max();
  }

  template<typename ValueType>
  inline ValueType Range<ValueType>::length() const noexcept
  {
    return max() - min();
  }

  template<typename ValueType>
  inline bool Range<ValueType>::contains(ValueType value) const noexcept
  {
    if(!isValid()) return false;
    return value >= min() && value <= max();
  }

  template<typename ValueType>
  inline void Range<ValueType>::set(ValueType min, ValueType max) noexcept
  {
    _min = std::min(min, max);
    _max = std::max(min, max);
    _valid = true;
  }

  template<typename ValueType>
  inline void Range<ValueType>::extend(ValueType value) noexcept
  {
    if (isValid())
    {
      _min = std::min(_min, value);
      _max = std::max(_max, value);
    }
    else
    {
      _min = value;
      _max = value;
      _valid = true;
    }
  }

  template<typename ValueType>
  inline void Range<ValueType>::extend(const Range<ValueType>& other) noexcept
  {
    if(!other.isValid()) return;

    if (isValid())
    {
      _min = std::min(_min, other._min);
      _max = std::max(_max, other._max);
    }
    else
    {
      *this = other;
    }
  }

  template<typename ValueType>
  inline bool Range<ValueType>::operator == (const Range& other) const noexcept
  {
    if(_valid == false and other._valid == false) return true;
    return _valid == other._valid && _min == other._min && _max == other._max;
  }
}