#pragma once

namespace mtt
{
  template <typename ValueType, ValueType setValue, ValueType resetValue>
  class ScopedSetter
  {
  public:
    inline ScopedSetter(ValueType& variable) noexcept;
    ScopedSetter(const ScopedSetter&) = delete;
    ScopedSetter& operator = (const ScopedSetter&) = delete;
    inline ~ScopedSetter() noexcept;

  private:
    ValueType& _variable;
  };

  typedef ScopedSetter<bool, true, false> ScopedTrueSetter;
  typedef ScopedSetter<bool, false, true> ScopedFalseSetter;

  template <typename ValueType, ValueType setValue, ValueType resetValue>
  inline ScopedSetter<ValueType, setValue, resetValue>::ScopedSetter(
                                                  ValueType& variable) noexcept:
    _variable(variable)
  {
    _variable = setValue;
  }

  template <typename ValueType, ValueType setValue, ValueType resetValue>
  inline ScopedSetter<ValueType, setValue, resetValue>::~ScopedSetter() noexcept
  {
    _variable = resetValue;
  }
}