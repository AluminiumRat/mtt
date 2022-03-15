#pragma once

#include <mtt/application/EditCommands/AbstractEditCommand.h>
#include <mtt/application/Scene/ObjectLink.h>

namespace mtt
{
  template <typename ObserverClass,
            typename ReferencedClass,
            ObjectRef<ReferencedClass>&(ObserverClass::*refGetter)() noexcept>
  class SetReferenceCommand : public AbstractEditCommand
  {
  public:
    inline SetReferenceCommand( ObserverClass& observer,
                                ReferencedClass* oldReference,
                                ReferencedClass* newReference) :
      _observer(observer),
      _oldReference(oldReference),
      _newReference(newReference)
    {
    }

    SetReferenceCommand(const SetReferenceCommand&) = delete;
    SetReferenceCommand& operator = (const SetReferenceCommand&) = delete;
    virtual ~SetReferenceCommand() noexcept = default;

    inline virtual void make() override
    {
      (_observer.*refGetter)().set(_newReference);
    }

    inline virtual void undo() override
    {
      (_observer.*refGetter)().set(_oldReference);
    }

  private:
    ObserverClass& _observer;
    ReferencedClass* _oldReference;
    ReferencedClass* _newReference;
  };
}