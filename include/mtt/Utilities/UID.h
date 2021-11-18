#pragma once

#include <cstdint>

namespace mtt
{
  class UID
  {
  public:
    inline UID();
    inline explicit UID(uint64_t value);
    UID(const UID& other) = default;
    UID& operator = (const UID& other) = default;
    ~UID() = default;

    inline bool isValid() const;
    inline uint64_t value() const;
    inline UID mixedUID(uint64_t mixingNumber) const;
    inline uint64_t mixedUIDValue(uint64_t mixingNumber) const;
    static uint64_t randomValue();

  private:
    uint64_t _value;
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

  inline UID::UID(uint64_t value) :
    _value(value)
  {
  }

  inline bool UID::isValid() const
  {
    return _value != 0;
  }

  inline uint64_t UID::value() const
  {
    return _value;
  }

  inline UID UID::mixedUID(uint64_t mixingNumber) const
  {
    return UID(_value ^ mixingNumber);
  }

  inline uint64_t UID::mixedUIDValue(uint64_t mixingNumber) const
  {
    return (_value ^ mixingNumber);
  }
}