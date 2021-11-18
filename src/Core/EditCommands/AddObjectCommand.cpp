#include <stdexcept>

#include <mtt/Core/EditCommands/AddObjectCommand.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

AddObjectCommand::AddObjectCommand( std::unique_ptr<Object> object,
                                    Object& group) :
  _object(std::move(object)),
  _objectRef(*_object),
  _group(group)
{
  if(!_group.subobjectCanBeAddedAndRemoved(_objectRef)) Abort("AddObjectCommand::AddObjectCommand: adding or removing is not supported.");
  if(_object.get() == nullptr) Abort("AddObjectCommand::AddObjectCommand: object is null.");
}

inline void AddObjectCommand::make()
{
  if(_object == nullptr) Abort("AddObjectCommand::make: object is null");
  _object = _group.tryAddSubobject(std::move(_object));
  if (_object != nullptr) throw(std::runtime_error("Unable to add subobject."));
}

inline void AddObjectCommand::undo()
{
  if (_object != nullptr) Abort("AddObjectCommand::make: object is not null");
  _object = _group.tryRemoveSubobject(_objectRef, true);
  if (_object == nullptr) throw(std::runtime_error("Unable to remove subobject."));
}
