#pragma once

namespace mtt
{
  template <typename ValueType, typename TimeType>
  class AbstractAnimatedValue
  {
  public:
    AbstractAnimatedValue() = default;
    AbstractAnimatedValue(const AbstractAnimatedValue&) = delete;
    AbstractAnimatedValue& operator = (const AbstractAnimatedValue&) = delete;
    virtual ~AbstractAnimatedValue() noexcept = default;
  
    virtual ValueType value(TimeType time) const = 0;
  };
}