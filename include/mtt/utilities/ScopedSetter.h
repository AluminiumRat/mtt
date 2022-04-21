#pragma once

namespace mtt
{
  template <typename ValueType>
  class ScopedSetter
  {
  public:
    ScopedSetter(ValueType& variable, const ValueType& setValue) noexcept :
      _variable(variable),
      _oldValue(variable)
    {
      _variable = setValue;
    }

    ScopedSetter(const ScopedSetter&) = delete;
    ScopedSetter& operator = (const ScopedSetter&) = delete;
    inline ~ScopedSetter() noexcept
    {
      _variable = _oldValue;
    }

  private:
    ValueType& _variable;
    ValueType _oldValue;
  };

  template <typename ValueType>
  class ValueRestorer
  {
  public:
    ValueRestorer(ValueType& variable) noexcept :
      _variable(variable),
      _oldValue(variable)
    {
    }

    ValueRestorer(const ValueRestorer&) = delete;
    ValueRestorer& operator = (const ValueRestorer&) = delete;
    inline ~ValueRestorer() noexcept
    {
      _variable = _oldValue;
    }

  private:
    ValueType& _variable;
    ValueType _oldValue;
  };
}