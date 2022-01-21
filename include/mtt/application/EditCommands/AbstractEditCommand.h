#pragma once

namespace mtt
{
  class AbstractEditCommand
  {
  public:
    AbstractEditCommand() = default;
    AbstractEditCommand(const AbstractEditCommand&) = delete;
    AbstractEditCommand& operator = (const AbstractEditCommand&) = delete;
    virtual ~AbstractEditCommand() noexcept = default;

    virtual void make() = 0;
    virtual void undo() = 0;
  };
}
