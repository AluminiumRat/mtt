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
                                ReferencedClass* newReference) :
      _observer(observer),
      _newReference(newReference != nullptr ? newReference->id() : UID())
    {
    }

    SetReferenceCommand(const SetReferenceCommand&) = delete;
    SetReferenceCommand& operator = (const SetReferenceCommand&) = delete;
    virtual ~SetReferenceCommand() noexcept = default;

    inline virtual void make() override
    {
      _oldReference = (_observer.*refGetter)().referencedId();
      (_observer.*refGetter)().setReferencedId(_newReference);
    }

    inline virtual void undo() override
    {
      (_observer.*refGetter)().setReferencedId(_oldReference);
    }

  private:
    ObserverClass& _observer;
    UID _oldReference;
    UID _newReference;
  };
}