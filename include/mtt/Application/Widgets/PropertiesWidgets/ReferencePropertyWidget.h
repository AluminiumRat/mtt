#pragma once

#include <mtt/Application/EditCommands/SetPropertyCommand.h>
#include <mtt/Application/EditCommands/UndoStack.h>
#include <mtt/Application/Scene/object_cast.h>
#include <mtt/Application/Scene/Object.h>
#include <mtt/Application/Widgets/PropertiesWidgets/ReferenceWidget.h>
#include <mtt/Application/Widgets/SelectObjectsDialog.h>
#include <mtt/Utilities/Abort.h>

namespace mtt
{
  template <typename ObserverClass,
            typename ReferencedClass>
  class ReferencePropertyWidget : public ReferenceWidget
  {
  public:
    using Getter = ReferencedClass* (ObserverClass::*)() const noexcept;
    using Setter = void (ObserverClass::*)(ReferencedClass*);

  public:
    template<typename Signal>
    inline ReferencePropertyWidget( ObserverClass& object,
                                    Getter getter,
                                    Setter setter,
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
    inline void _setReference(ReferencedClass* object) noexcept;

  private:
    ObserverClass& _object;
    Getter _getter;
    Setter _setter;

    ReferencedClass* _referenced;

    UndoStack& _undoStack;

    Object& _selectArea;
  };

  template< typename ObserverClass,
            typename ReferencedClass>
  template<typename Signal>
  inline ReferencePropertyWidget< ObserverClass,
                                  ReferencedClass>::
                                          ReferencePropertyWidget(
                                                        ObserverClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack,
                                                        Object& selectArea) :
    _object(object),
    _getter(getter),
    _setter(setter),
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
            typename ReferencedClass>
  inline void ReferencePropertyWidget<ObserverClass, ReferencedClass>::
                                                  _connectToReference() noexcept
  {
    try
    {
      ReferencedClass* newReferenced = (_object.*_getter)();
      if(newReferenced == _referenced) return;

      if(_referenced != nullptr)
      {
        disconnect( _referenced,
                    &Object::objectNameChanged,
                    this,
                    &ReferencePropertyWidget::_updateName);
      }
      
      _referenced = newReferenced;

      if (_referenced != nullptr)
      {
        connect(_referenced,
                &Object::objectNameChanged,
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
            typename ReferencedClass>
  inline void ReferencePropertyWidget<ObserverClass,
                                      ReferencedClass>::_updateName() noexcept
  {
    try
    {
      if(_referenced == nullptr) setNameLabel(tr("none"));
      else setNameLabel(_referenced->objectName());
    }
    catch (...)
    {
      Abort("Vec3PropertyWidget::_updateName: unable to update widget.");
    }
  }

  template< typename ObserverClass,
            typename ReferencedClass>
  inline void ReferencePropertyWidget<ObserverClass, ReferencedClass>::
                                                      selectReference() noexcept
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
            typename ReferencedClass>
  inline void ReferencePropertyWidget< ObserverClass, ReferencedClass>::
                                                      resetReference() noexcept
  {
    _setReference(nullptr);
  }

  template< typename ObserverClass,
            typename ReferencedClass>
  inline void ReferencePropertyWidget<ObserverClass, ReferencedClass>::
                                _setReference(ReferencedClass* object) noexcept
  {
    if((_object.*_getter)() == object) return;

    try
    {
      using Command = SetPropertyCommand< ObserverClass,
                                          ReferencedClass*,
                                          Setter>;
      std::unique_ptr<Command> command(new Command( _object,
                                                    _setter,
                                                    (_object.*_getter)(),
                                                    object));
      _undoStack.addAndMake(std::move(command));
    }
    catch(...)
    {
      Abort("ReferencePropertyWidget::setReference: unable to update reference.");
    }
  }
}