#pragma once

#include <mtt/application/EditCommands/AbstractEditCommand.h>

namespace mtt
{
  class Object;

  class MoveObjectCommand : public AbstractEditCommand
  {
  public:
    MoveObjectCommand(Object& object, Object& newParent);
    MoveObjectCommand(const MoveObjectCommand&) = delete;
    MoveObjectCommand& operator = (const MoveObjectCommand&) = delete;
    virtual ~MoveObjectCommand() noexcept = default;

    virtual void make() override;
    virtual void undo() override;
  
  private:
    Object& _object;
    Object& _oldParent;
    Object& _newParent;
  };
}