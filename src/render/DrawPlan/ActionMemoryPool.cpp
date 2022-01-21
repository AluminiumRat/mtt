#include <mtt/render/DrawPlan/ActionMemoryPool.h>

using namespace mtt;

ActionMemoryPool::ActionMemoryPool(size_t initialSize) :
  _holderSize(initialSize),
  _currentHolderIndex(0),
  _currentHolder(nullptr)
{
}

void ActionMemoryPool::reset() noexcept
{
  if(_holders.empty()) return;
  for(size_t iHolder = 0; iHolder < _holders.size(); iHolder++)
  {
    _holders[iHolder]->reset();
  }
  selectHolder(0);
}

void ActionMemoryPool::addHolder()
{
  std::unique_ptr<ActionMemoryHolder>
                                newHolder(new ActionMemoryHolder(_holderSize));
  _holders.push_back(std::move(newHolder));
}

void ActionMemoryPool::selectHolder(size_t holderIndex) noexcept
{
  _currentHolder = _holders[holderIndex].get();
  _currentHolderIndex = holderIndex;
}
