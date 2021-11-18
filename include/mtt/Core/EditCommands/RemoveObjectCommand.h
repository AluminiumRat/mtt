#pragma once

#include <memory>

#include <mtt/Core/EditCommands/AbstractEditCommand.h>
#include <mtt/Core/Scene/Object.h>

namespace mtt
{
  class RemoveObjectCommand : public AbstractEditCommand
  {
  public:
    RemoveObjectCommand(Object& object, Object& group);
    RemoveObjectCommand(const RemoveObjectCommand&) = delete;
    RemoveObjectCommand& operator = (const RemoveObjectCommand&) = delete;
    virtual ~RemoveObjectCommand() noexcept = default;

    virtual void make() override;
    virtual void undo() override;
  
  private:
    std::unique_ptr<Object> _object;
    Object& _objectRef;
    Object& _group;
  };
}