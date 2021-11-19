#include <stdexcept>

#include <mtt/Application/EditCommands/RemoveObjectCommand.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

RemoveObjectCommand::RemoveObjectCommand(Object& object, Object& group) :
  _objectRef(object),
  _group(group)
{
  if(!_group.subobjectCanBeAddedAndRemoved(_objectRef)) Abort("RemoveObjectCommand::RemoveObjectCommand: adding or removing is not supported.");
}

inline void RemoveObjectCommand::make()
{
  if (_object != nullptr) Abort("RemoveObjectCommand::make: object is not null");
  _object = _group.tryRemoveSubobject(_objectRef, true);
  if (_object == nullptr) throw(std::runtime_error("Unable to remove subobject."));
}

inline void RemoveObjectCommand::undo()
{
  if (_object == nullptr) Abort("RemoveObjectCommand::make: object is null");
  _object = _group.tryAddSubobject(std::move(_object));
  if (_object != nullptr) throw(std::runtime_error("Unable to add subobject."));
}
