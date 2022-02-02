#pragma once

#include <vector>

#include <mtt/render/SceneGraph/AreaModificator.h>

namespace mtt
{
  class AreaModificatorSet
  {
  public:
    AreaModificatorSet() = default;
    inline AreaModificatorSet(std::vector<AreaModificator*>&& modificators);
    AreaModificatorSet(const AreaModificatorSet&) = delete;
    AreaModificatorSet& operator = (const AreaModificatorSet&) = delete;
    ~AreaModificatorSet() noexcept = default;

    inline const std::vector<AreaModificator*>& modificators() const noexcept;

  private:
    std::vector<AreaModificator*> _modificators;
  };

  inline AreaModificatorSet::AreaModificatorSet(
                                std::vector<AreaModificator*>&& modificators) :
    _modificators(std::move(modificators))
  {
  }

  inline const std::vector<AreaModificator*>&
                              AreaModificatorSet::modificators() const noexcept
  {
    return _modificators;
  }
}