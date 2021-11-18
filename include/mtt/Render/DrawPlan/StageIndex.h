#pragma once

#include <compare>

namespace mtt
{
  class StageIndex
  {
  public:
    constexpr explicit StageIndex(int index) noexcept :
      _index(index)
    {
    }

    constexpr StageIndex(const StageIndex&) noexcept = default;
    constexpr StageIndex& operator = (const StageIndex&) noexcept = default;
    ~StageIndex() noexcept = default;

    constexpr int index() const noexcept
    {
      return _index;
    }

    std::strong_ordering operator <=> (
                                    const StageIndex&) const noexcept = default;

  private:
    int _index;
  };

  namespace baseStages
  {
    /// This is not a general stage of the plan,
    /// this is pre- or post- substage of
    /// another stages. There is no draw bin for
    /// this stage in the frame plan.
    constexpr StageIndex subsidiaryStage(-1);

    /// Single stage for uid render
    constexpr StageIndex uidStage(0);

    /// Single stage for depthmap render
    constexpr StageIndex depthmapStage(0);
  }
};