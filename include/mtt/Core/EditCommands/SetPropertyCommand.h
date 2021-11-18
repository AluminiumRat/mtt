#pragma once

#include <mtt/Core/EditCommands/AbstractEditCommand.h>

namespace mtt
{
  template <typename ObjectClass,
            typename PropertyType,
            typename Setter>
  class SetPropertyCommand : public AbstractEditCommand
  {
  public:
    inline SetPropertyCommand(ObjectClass& object,
                              Setter setter,
                              const PropertyType& oldValue,
                              const PropertyType& newValue) :
      _object(object),
      _setter(setter),
      _newValue(newValue),
      _oldValue(oldValue)
    {
    }
    SetPropertyCommand(const SetPropertyCommand&) = delete;
    SetPropertyCommand& operator = (const SetPropertyCommand&) = delete;
    virtual ~SetPropertyCommand() noexcept = default;

    inline virtual void make() override
    {
      (_object.*_setter)(_newValue);
    }

    inline virtual void undo() override
    {
      (_object.*_setter)(_oldValue);
    }
  
  private:
    ObjectClass& _object;
    Setter _setter;
    PropertyType _newValue;
    PropertyType _oldValue;
  };
}