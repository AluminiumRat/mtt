#pragma once

#include <memory>
#include <vector>

#include <mtt/render/DrawPlan/ActionMemoryHolder.h>

namespace mtt
{
  class ActionMemoryPool
  {
  public:
    ActionMemoryPool(size_t initialSize);
    ActionMemoryPool(const ActionMemoryPool&) = delete;
    ActionMemoryPool& operator = (const ActionMemoryPool&) = delete;
    ~ActionMemoryPool() noexcept = default;

    template<typename ActionType, typename... Args>
    inline ActionPtr emplace(Args&... args);

    void reset() noexcept;

  private:
    void addHolder();
    void selectHolder(size_t holderIndex) noexcept;

  private:
    size_t _holderSize;
    using Holders = std::vector<std::unique_ptr<ActionMemoryHolder>>;
    Holders _holders;
    size_t _currentHolderIndex;
    ActionMemoryHolder* _currentHolder;
  };

  template<typename ActionType, typename... Args>
  inline ActionPtr ActionMemoryPool::emplace(Args&... args)
  {
    if(_holders.empty())
    {
      addHolder();
      selectHolder(0);
    }

    if(_currentHolder->memoryLeft() < sizeof(ActionType))
    {
      if(_currentHolderIndex == _holders.size() - 1) addHolder();
      selectHolder(_currentHolderIndex + 1);
    }

    return _currentHolder->emplace<ActionType>(args...);
  }
}