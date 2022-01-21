#pragma once

#include <memory>

#include <mtt/application/EditCommands/AbstractEditCommand.h>
#include <mtt/application/Scene/Object.h>

namespace mtt
{
  class AddObjectCommand : public AbstractEditCommand
  {
  public:
    AddObjectCommand(std::unique_ptr<Object> object,
                            Object& group);
    AddObjectCommand(const AddObjectCommand&) = delete;
    AddObjectCommand& operator = (const AddObjectCommand&) = delete;
    virtual ~AddObjectCommand() noexcept = default;

    virtual void make() override;
    virtual void undo() override;
  
  private:
    std::unique_ptr<Object> _object;
    Object& _objectRef;
    Object& _group;
  };
}