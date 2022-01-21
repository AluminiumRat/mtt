#pragma once

#include <mtt/render/DrawPlan/DrawContext.h>

namespace mtt
{
  class AbstractAction
  {
  public:
    AbstractAction() noexcept = default;
    AbstractAction(const AbstractAction&) = delete;
    AbstractAction& operator = (const AbstractAction&) = delete;
    virtual ~AbstractAction() noexcept = default;

    virtual void schedule(DrawContext& context) = 0;
  };
}