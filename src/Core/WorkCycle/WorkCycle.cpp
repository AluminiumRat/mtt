#include <future>

#include <mtt/Core/WorkCycle/WorkCycle.h>
#include <mtt/Utilities/Abort.h>
#include <mtt/Utilities/ScopedSetter.h>

using namespace mtt;

WorkCycle::WorkCycle() noexcept :
  _inProgress(false)
{
}

void WorkCycle::addAction(std::unique_ptr<AbstractAction> action,
                          int category,
                          int priority,
                          bool permanent)
{
  if(_inProgress) Abort("WorkCycle::addAction: addAction should be called outside of the step method.");

  Actions::iterator insertPosition = _actions.begin();
  while(insertPosition != _actions.end() &&
        insertPosition->priority >= priority)
  {
    insertPosition++;
  }
  Actions::iterator newRecord = _actions.emplace(insertPosition);
  newRecord->action = std::move(action);
  newRecord->category = std::move(category);
  newRecord->priority = std::move(priority);
  newRecord->permanent = std::move(permanent);
}

std::unique_ptr<WorkCycle::AbstractAction> WorkCycle::removeAction(
                                                AbstractAction& action) noexcept
{
  if(_inProgress) Abort("WorkCycle::removeAction: removeAction should be called outside of the step method.");

  for(Actions::iterator iAction = _actions.begin();
      iAction != _actions.end();
      iAction++)
  {
    if(iAction->action.get() == &action)
    {
      std::unique_ptr<WorkCycle::AbstractAction> removedAction =
                                                    std::move(iAction->action);
      _actions.erase(iAction);
      return removedAction;
    }
  }

  return nullptr;
}

void WorkCycle::removeSingleshots() noexcept
{
  Actions::iterator iAction = _actions.begin();
  while(iAction != _actions.end())
  {
    if(!iAction->permanent) iAction = _actions.erase(iAction);
    else iAction++;
  }
}

void WorkCycle::step()
{
  ScopedTrueSetter setInProgress(_inProgress);
  
  std::vector<AbstractAction*> groupTable(totalCategories, nullptr);
  std::vector<AbstractAction*> group;
  group.reserve(totalCategories);

  Actions::iterator cursor = _actions.begin();
  while(cursor != _actions.end())
  {
    for(AbstractAction*& tableRec : groupTable) tableRec = nullptr;
    group.clear();

    while(cursor != _actions.end() &&
          groupTable[cursor->category] == nullptr)
    {
      groupTable[cursor->category] = cursor->action.get();
      group.push_back(cursor->action.get());
      cursor++;
    }
    doGroup(group);
  }  

  removeSingleshots();
}

void WorkCycle::doGroup(const std::vector<AbstractAction*>& group)
{
  for(AbstractAction* action : group)
  {
    if(action != nullptr) action->preAsycPart();
  }

  std::vector<std::future<void>> futures;
  for (AbstractAction* action : group)
  {
    if (action != nullptr)
    {
      futures.push_back(std::async([=](){action->asycPart();}));
    }
  }
  for(std::future<void>& future : futures) future.get();

  for (AbstractAction* action : group)
  {
    if (action != nullptr) action->postAsycPart();
  }
}
