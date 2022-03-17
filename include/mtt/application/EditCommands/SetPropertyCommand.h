#pragma once

#include <memory>
#include <mtt/application/EditCommands/AbstractEditCommand.h>

namespace mtt
{
  template <typename ObjectClass,
            typename PropertyType,
            typename Getter,
            typename Setter>
  class SetPropertyCommand : public AbstractEditCommand
  {
  public:
    inline SetPropertyCommand(ObjectClass& object,
                              Getter getter,
                              Setter setter,
                              const PropertyType& newValue) :
      _object(object),
      _getter(getter),
      _setter(setter),
      _newValue(newValue)
    {
    }
    SetPropertyCommand(const SetPropertyCommand&) = delete;
    SetPropertyCommand& operator = (const SetPropertyCommand&) = delete;
    virtual ~SetPropertyCommand() noexcept = default;

    inline virtual void make() override
    {
      _oldValue = (_object.*_getter)();
      (_object.*_setter)(_newValue);
    }

    inline virtual void undo() override
    {
      (_object.*_setter)(_oldValue);
    }

  private:
    ObjectClass& _object;
    Getter _getter;
    Setter _setter;
    PropertyType _newValue;
    PropertyType _oldValue;
  };

  template <typename ObjectClass,
            typename PropertyType,
            typename Getter,
            typename Setter>
  std::unique_ptr<AbstractEditCommand> makeSetPropertyCommand(
                                                  ObjectClass& object,
                                                  Getter getter,
                                                  Setter setter,
                                                  const PropertyType& newValue)
  {
    using CommandType = SetPropertyCommand< ObjectClass,
                                            PropertyType,
                                            Getter,
                                            Setter>;
    return std::make_unique<CommandType>(object, getter, setter, newValue);
  }
}