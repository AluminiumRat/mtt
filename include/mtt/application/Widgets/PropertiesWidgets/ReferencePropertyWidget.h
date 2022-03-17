#pragma once

#include <mtt/application/EditCommands/SetReferenceCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/application/Widgets/PropertiesWidgets/ReferenceWidget.h>
#include <mtt/application/Widgets/SelectObjectsDialog.h>
#include <mtt/utilities/Abort.h>

namespace mtt
{
  template <typename ObserverClass,
            typename ReferencedClass,
            ObjectRef<ReferencedClass>& (ObserverClass::* refGetter)() noexcept>
  class ReferencePropertyWidget : public ReferenceWidget
  {
  public:
    template<typename Signal>
    inline ReferencePropertyWidget( ObserverClass& object,
                                    Signal signal,
                                    UndoStack& undoStack,
                                    Object& selectArea);
    ReferencePropertyWidget(const ReferencePropertyWidget&) = delete;
    ReferencePropertyWidget& operator =
                                      (const ReferencePropertyWidget&) = delete;
    virtual ~ReferencePropertyWidget() noexcept = default;

  protected:
    inline virtual void selectReference() noexcept override;
    inline virtual void resetReference() noexcept override;

  private:
    inline void _connectToReference() noexcept;
    inline void _updateName() noexcept;
    inline void _setReference(ReferencedClass* newValue) noexcept;

  private:
    ObserverClass& _object;
    ObjectRef<ReferencedClass>& _reference;

    ReferencedClass* _referenced;

    UndoStack& _undoStack;

    Object& _selectArea;
  };

  template< typename ObserverClass,
            typename ReferencedClass,
            ObjectRef<ReferencedClass>& (ObserverClass::* refGetter)() noexcept>
  template<typename Signal>
  inline ReferencePropertyWidget< ObserverClass,
                                  ReferencedClass,
                                  refGetter>::ReferencePropertyWidget(
                                          ObserverClass& object,
                                          Signal signal,
                                          UndoStack& undoStack,
                                          Object& selectArea) :
    _object(object),
    _reference((object.*refGetter)()),
    _referenced(nullptr),
    _undoStack(undoStack),
    _selectArea(selectArea)
  {
    connect(&_object,
            signal,
            this,
            &ReferencePropertyWidget::_connectToReference,
            Qt::DirectConnection);
    _connectToReference();
    _updateName();
  }

  template< typename ObserverClass,
            typename ReferencedClass,
            ObjectRef<ReferencedClass>& (ObserverClass::* refGetter)() noexcept>
  inline void ReferencePropertyWidget<ObserverClass,
                                      ReferencedClass,
                                      refGetter>::_connectToReference() noexcept
  {
    try
    {
      ReferencedClass* newReferenced = _reference.get();
      if(newReferenced == _referenced) return;

      if(_referenced != nullptr)
      {
        disconnect( _referenced,
                    &Object::nameChanged,
                    this,
                    &ReferencePropertyWidget::_updateName);
      }
      
      _referenced = newReferenced;

      if (_referenced != nullptr)
      {
        connect(_referenced,
                &Object::nameChanged,
                this,
                &ReferencePropertyWidget::_updateName,
                Qt::DirectConnection);
      }

      _updateName();
    }
    catch (...)
    {
      Abort("Vec3PropertyWidget::_connectToReference: unable to update reference.");
    }
  }

  template< typename ObserverClass,
            typename ReferencedClass,
            ObjectRef<ReferencedClass>& (ObserverClass::* refGetter)() noexcept>
  inline void ReferencePropertyWidget<ObserverClass,
                                      ReferencedClass,
                                      refGetter>::_updateName() noexcept
  {
    try
    {
      if(_referenced == nullptr) setNameLabel(tr("none"));
      else setNameLabel(_referenced->name());
    }
    catch (...)
    {
      Abort("Vec3PropertyWidget::_updateName: unable to update widget.");
    }
  }

  template< typename ObserverClass,
            typename ReferencedClass,
            ObjectRef<ReferencedClass>& (ObserverClass::* refGetter)() noexcept>
  inline void ReferencePropertyWidget<ObserverClass,
                                      ReferencedClass,
                                      refGetter>::selectReference() noexcept
  {
    auto filter = [](Object& object) -> bool
    {
      return qobject_cast<ReferencedClass*>(&object) != nullptr;
    };

    SelectObjectsDialog selectDialog( _selectArea,
                                      false,
                                      std::move(filter),
                                      this);
    selectDialog.exec();
    if(selectDialog.selected().empty()) return;

    ReferencedClass* newRefernce = static_cast<ReferencedClass*>(
                                                    selectDialog.selected()[0]);
    _setReference(newRefernce);
  }

  template< typename ObserverClass,
            typename ReferencedClass,
            ObjectRef<ReferencedClass>& (ObserverClass::* refGetter)() noexcept>
  inline void ReferencePropertyWidget<ObserverClass,
                                      ReferencedClass,
                                      refGetter>::resetReference() noexcept
  {
    _setReference(nullptr);
  }

  template< typename ObserverClass,
            typename ReferencedClass,
            ObjectRef<ReferencedClass>& (ObserverClass::* refGetter)() noexcept>
  inline void ReferencePropertyWidget<ObserverClass,
                                      ReferencedClass,
                                      refGetter>::_setReference(
                                              ReferencedClass* newValue) noexcept
  {
    if(_reference.get() == newValue) return;

    try
    {
      using Command = SetReferenceCommand<ObserverClass,
                                          ReferencedClass,
                                          refGetter>;
      std::unique_ptr<Command> command(new Command(_object, newValue));
      _undoStack.addAndMake(std::move(command));
    }
    catch(...)
    {
      Abort("ReferencePropertyWidget::setReference: unable to update reference.");
    }
  }
}