#include <mtt/Application/EditCommands/CompositeCommand.h>

using namespace mtt;

void CompositeCommand::make()
{
  for(Commands::iterator iCommand = _commands.begin();
      iCommand != _commands.end();
      iCommand++)
  {
    iCommand->get()->make();
  }
}

void CompositeCommand::undo()
{
  for(Commands::reverse_iterator iCommand = _commands.rbegin();
      iCommand != _commands.rend();
      iCommand++)
  {
    iCommand->get()->undo();
  }
}
