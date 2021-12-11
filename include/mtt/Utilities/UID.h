#pragma once

#include <cstdint>

namespace mtt
{
  class UID
  {
  public:
    using ValueType = uint64_t;

  public:
    inline UID();
    inline explicit UID(ValueType value);
    UID(const UID& other) = default;
    UID& operator = (const UID& other) = default;
    ~UID() = default;

    inline bool isValid() const;
    inline ValueType value() const;
    inline UID mixedUID(ValueType mixingNumber) const;
    inline ValueType mixedUIDValue(ValueType mixingNumber) const;
    static ValueType randomValue();

  private:
    ValueType _value;
  };

  inline bool operator == (const UID& uidLeft, const UID& uidRight) noexcept
  {
    return (uidLeft.value() == uidRight.value());
  }

  inline bool operator != (const UID& uidLeft, const UID& uidRight) noexcept
  {
    return !(uidLeft == uidRight);
  }

  inline bool operator < (const UID& uidLeft, const UID& uidRight) noexcept
  {
    return (uidLeft.value() < uidRight.value());
  }

  inline bool operator > (const UID& uidLeft, const UID& uidRight) noexcept
  {
    return (uidRight < uidLeft);
  }

  inline bool operator <= (const UID& uidLeft, const UID& uidRight) noexcept
  {
    return !(uidLeft > uidRight);
  }

  inline bool operator >= (const UID& uidLeft, const UID& uidRight) noexcept
  {
    return !(uidLeft < uidRight);
  }

  inline UID::UID() :
    _value(0)
  {
  }

  inline UID::UID(ValueType value) :
    _value(value)
  {
  }

  inline bool UID::isValid() const
  {
    return _value != 0;
  }

  inline UID::ValueType UID::value() const
  {
    return _value;
  }

  inline UID UID::mixedUID(ValueType mixingNumber) const
  {
    return UID(_value ^ mixingNumber);
  }

  inline UID::ValueType UID::mixedUIDValue(ValueType mixingNumber) const
  {
    return (_value ^ mixingNumber);
  }
}