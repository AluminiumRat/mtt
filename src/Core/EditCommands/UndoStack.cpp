#include <stdexcept>

#include <mtt/Core/EditCommands/UndoStack.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

UndoStack::UndoStack( size_t stackSize) :
  _stackSize(stackSize),
  _cursor(_commands.end()),
  _undoAvailable(false),
  _redoAvailable(false),
  _substackUseCounter(0)
{
}

void UndoStack::_updateAvailables() noexcept
{
  bool oldUndoAvailable = _undoAvailable;
  bool oldRedoAvailable = _redoAvailable;

  if(_substack != nullptr)
  {
    _undoAvailable = _substack->undoAvailable();
    _redoAvailable = _substack->redoAvailable();
  }
  else
  {
    _undoAvailable = _cursor != _commands.begin();
    _redoAvailable = _cursor != _commands.end();
  }

  if(oldUndoAvailable != _undoAvailable)
  {
    emit undoAvailableChanged(_undoAvailable);
  }

  if(oldRedoAvailable != _redoAvailable)
  {
    emit redoAvailableChanged(_redoAvailable);
  }
}

void UndoStack::addAndMake(std::unique_ptr<AbstractEditCommand> command)
{
  try
  {
    command->make();
    addCommand(std::move(command));
  }
  catch(...)
  {
    clear();
    throw;
  }
}

void UndoStack::addCommand(std::unique_ptr<AbstractEditCommand> command)
{
  if(_substack == nullptr)
  {
    _dropTail();
    _commands.push_back(std::move(command));
    if(_stackSize != 0 && _commands.size() > _stackSize) _commands.pop_front();
    _cursor = _commands.end();
    _updateAvailables();
  }
  else _substack->addCommand(std::move(command));
}

void UndoStack::_dropTail()
{
  _commands.erase(_cursor, _commands.end());
}

void UndoStack::undoCommand()
{
  if (!undoAvailable()) return;
  try
  {
    if(_substack == nullptr)
    {
      Commands::iterator undoCursor = _cursor - 1;
      (*undoCursor)->undo();
      _cursor = undoCursor;
    }
    else _substack->undoCommand();
  }
  catch(...)
  {
    clear();
    _updateAvailables();
    throw;
  }
  _updateAvailables();
}

void UndoStack::redoCommand()
{
  if (!redoAvailable()) return;
  try
  {
    if(_substack == nullptr)
    {
      (*_cursor)->make();
      _cursor++;
    }
    else _substack->redoCommand();
  }
  catch(...)
  {
    clear();
    _updateAvailables();
    throw;
  }

  _updateAvailables();
}

void UndoStack::clear() noexcept
{
  try
  {
    if(_substack != nullptr) _substack->clear();
    _commands.clear();
    _cursor = _commands.end();
    _updateAvailables();
  }
  catch(std::exception& error)
  {
    Log() << "UndoStack::clear: " << error.what();
    Abort("UndoStack::clear: unable to clear commands queue.");
  }
  catch (...)
  {
    Abort("UndoStack::clear: unable to clear commands queue.");
  }
}

void UndoStack::make()
{
  if(_substack != nullptr) Abort("UndoStack::make: _substack != nullptr");

  for(Commands::iterator iCommand = _commands.begin();
      iCommand != _cursor;
      iCommand++)
  {
    (*iCommand)->make();
  }
}

void UndoStack::undo()
{
  if(_substack != nullptr) Abort("UndoStack::make: _substack != nullptr");

  for(Commands::reverse_iterator iCommand = std::reverse_iterator(_cursor);
      iCommand != _commands.rend();
      iCommand++)
  {
    (*iCommand)->undo();
  }
}

void UndoStack::lockGroup()
{
  if(_substack != nullptr)
  {
    if(_substackUseCounter == 0) Abort("UndoStack::lockGroup: _substackUseCounter == 0");
    _substackUseCounter++;
    return;
  }

  _substack.reset(new UndoStack(0));
  _substackUseCounter = 1;

  connect(_substack.get(),
          &UndoStack::undoAvailableChanged,
          this,
          &UndoStack::_updateAvailables,
          Qt::DirectConnection);

  connect(_substack.get(),
          &UndoStack::redoAvailableChanged,
          this,
          &UndoStack::_updateAvailables,
          Qt::DirectConnection);
  _updateAvailables();
}

void UndoStack::ulockGroup() noexcept
{
  if(_substack == nullptr) Abort("UndoStack::ulockGroup: _substack == nullptr");
  if(_substackUseCounter == 0) Abort("UndoStack::ulockGroup: _substackUseCounter == 0");

  _substackUseCounter--;
  if(_substackUseCounter == 0)
  {
    try
    {
      disconnect( _substack.get(),
                  &UndoStack::undoAvailableChanged,
                  this,
                  &UndoStack::_updateAvailables);

      disconnect( _substack.get(),
                  &UndoStack::redoAvailableChanged,
                  this,
                  &UndoStack::_updateAvailables);

      _substack->_dropTail();
      if(_substack->undoAvailable())
      {
        addCommand(std::unique_ptr<AbstractEditCommand>(_substack.release()));
      }
      else _substack.release();
    }
    catch (std::exception& error)
    {
      Log() << "UndoStack::ulockGroup: " << error.what();
      Abort("UndoStack::ulockGroup: unable to unlock group.");
    }
    catch (...)
    {
      Abort("UndoStack::ulockGroup: unknown error.");
    }

    _updateAvailables();
  }
}
