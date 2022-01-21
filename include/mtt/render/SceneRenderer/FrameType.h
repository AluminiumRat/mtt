#pragma once

#include <compare>

namespace mtt
{
  class FrameType
  {
  public:
    constexpr explicit FrameType(size_t index) noexcept :
      _index(index)
    {
    }

    constexpr FrameType(const FrameType&) noexcept = default;
    constexpr FrameType& operator = (const FrameType&) noexcept = default;
    ~FrameType() noexcept = default;

    constexpr size_t index() const noexcept
    {
      return _index;
    }

    std::strong_ordering operator <=> (
                                    const FrameType&) const noexcept = default;

  private:
    size_t _index;
  };

  namespace baseFrameTypes
  {
    constexpr FrameType colorFrameType(0);
    constexpr FrameType uidFrameType(1);
    constexpr FrameType depthmapFrameType(2);
  }
};