#pragma once

#include <list>
#include <memory>
#include <vector>

namespace mtt
{
  class WorkCycle
  {
  public:
    enum ActionPriority
    {
      PRERENDER_PRIORITY = 3000,
      RENDER_PRIORITY = 2000,
      POSTRENDER_PRIORITY = 1000
    };

  public:
    class AbstractAction
    {
    public:
      AbstractAction() noexcept = default;
      AbstractAction(const AbstractAction&) = delete;
      AbstractAction& operator = (const AbstractAction&) = delete;
      virtual ~AbstractAction() noexcept = default;
    
      virtual void preAsycPart() = 0;
      virtual void asycPart() = 0;
      virtual void postAsycPart() = 0;
    };

  public:
    WorkCycle() noexcept;
    WorkCycle(const WorkCycle&) = delete;
    WorkCycle& operator = (const WorkCycle&) = delete;
    virtual ~WorkCycle() noexcept = default;

    /// Returns unique index for new category
    static int createActionCategory() noexcept;

    /// Should be called outside of the step method.
    void addAction( std::unique_ptr<AbstractAction> action,
                    int category,
                    int priority,
                    bool permanent);
    /// Should be called outside of the step method.
    std::unique_ptr<AbstractAction> removeAction(
                                              AbstractAction& action) noexcept;
    virtual void step() noexcept;

  private:
    struct ActionRecord
    {
      std::unique_ptr<AbstractAction> action;
      int category;
      int priority;
      bool permanent;
    };
    using Actions = std::list<ActionRecord>;

  private:
    void removeSingleshots() noexcept;
    void doGroup(const std::vector<AbstractAction*>& group);

  private:
    Actions _actions;

    bool _inProgress;
  };
}