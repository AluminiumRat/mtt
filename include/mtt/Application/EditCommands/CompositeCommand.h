#pragma once

#include <memory>
#include <vector>

#include <mtt/Application/EditCommands/AbstractEditCommand.h>

namespace mtt
{
  class CompositeCommand : public AbstractEditCommand
  {
  public:
    CompositeCommand() = default;
    CompositeCommand(const CompositeCommand&) = delete;
    CompositeCommand& operator = (const CompositeCommand&) = delete;
    virtual ~CompositeCommand() noexcept = default;

    virtual void make() override;
    virtual void undo() override;

    inline void addSubcommand(std::unique_ptr<AbstractEditCommand> command);

  private:
    using Commands = std::vector<std::unique_ptr<AbstractEditCommand>>;
    Commands _commands;
  };

  inline void CompositeCommand::addSubcommand(
                                  std::unique_ptr<AbstractEditCommand> command)
  {
    _commands.push_back(std::move(command));
  }
}