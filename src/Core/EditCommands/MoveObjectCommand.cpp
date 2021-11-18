#include <memory>
#include <stdexcept>

#include <mtt/Core/EditCommands/MoveObjectCommand.h>
#include <mtt/Core/Scene/Object.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

MoveObjectCommand::MoveObjectCommand(Object& object, Object& newParent) :
  _object(object),
  _oldParent(*object.parent()),
  _newParent(newParent)
{
  if(object.parent() == nullptr) Abort("MoveObjectCommand::MoveObjectCommand: objects parent is null.");
  if (!_oldParent.subobjectCanBeAddedAndRemoved(_object)) Abort("MoveObjectCommand::MoveObjectCommand: adding or removing is not supported in old parent.");
  if (!_newParent.subobjectCanBeAddedAndRemoved(_object)) Abort("MoveObjectCommand::MoveObjectCommand: adding or removing is not supported in new parent.");
}

inline void MoveObjectCommand::make()
{
  if (_object.parent() != &_oldParent) Abort("MoveObjectCommand::make: wrong parent.");
  std::unique_ptr<Object> objectKeeper =
                                  _oldParent.tryRemoveSubobject(_object, false);
  if(objectKeeper == nullptr) throw std::runtime_error("Unable to remove subobject from group.");
  objectKeeper = _newParent.tryAddSubobject(std::move(objectKeeper));
  if (objectKeeper != nullptr) throw std::runtime_error("Unable to insert subobject to group.");
}

inline void MoveObjectCommand::undo()
{
  if (_object.parent() != &_newParent) Abort("MoveObjectCommand::make: wrong parent.");
  std::unique_ptr<Object> objectKeeper =
                                  _newParent.tryRemoveSubobject(_object, false);
  if(objectKeeper == nullptr) throw std::runtime_error("Unable to remove subobject from group.");
  objectKeeper = _oldParent.tryAddSubobject(std::move(objectKeeper));
  if (objectKeeper != nullptr) throw std::runtime_error("Unable to insert subobject to group.");
}
