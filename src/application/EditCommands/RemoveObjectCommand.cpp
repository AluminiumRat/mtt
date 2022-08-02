#include <stdexcept>

#include <mtt/application/EditCommands/RemoveObjectCommand.h>
#include <mtt/utilities/Abort.h>

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
  if (_object == nullptr)
  {
    std::string errorString("RemoveObjectCommand:: Unable to remove ");
    errorString += _objectRef.name().toLocal8Bit().data();
    errorString += " from ";
    errorString += _group.name().toLocal8Bit().data();
    throw(std::runtime_error(errorString));
  }
}

inline void RemoveObjectCommand::undo()
{
  if (_object == nullptr) Abort("RemoveObjectCommand::make: object is null");
  _object = _group.tryAddSubobject(std::move(_object));
  if (_object != nullptr)
  {
    std::string errorString("RemoveObjectCommand:: Unable to add ");
    errorString += _objectRef.name().toLocal8Bit().data();
    errorString += " to ";
    errorString += _group.name().toLocal8Bit().data();
    throw(std::runtime_error(errorString));
  }
}
