#pragma once

#include <QtWidgets/QSpinBox>

#include <mtt/Application/EditCommands/SetPropertyCommand.h>
#include <mtt/Application/EditCommands/UndoStack.h>
#include <mtt/Utilities/Abort.h>
#include <mtt/Utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass, typename PropertyType>
  class IntPropertySpin : public QSpinBox
  {
  public:
    using Getter = PropertyType(ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(PropertyType);

  public:
    template<typename Signal>
    inline IntPropertySpin( ObjectClass& object,
                            Getter getter,
                            Setter setter,
                            Signal signal,
                            UndoStack& undoStack);
    IntPropertySpin(const IntPropertySpin&) = delete;
    IntPropertySpin& operator = (const IntPropertySpin&) = delete;
    virtual ~IntPropertySpin() noexcept = default;
  
    inline void setMaximum(int max);
    inline void setMinimum(int min);

  private:
    inline void updateProperty() noexcept;
    inline void updateWidget() noexcept;
  
  private:
    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    UndoStack& _undoStack;
    bool _skipUpdate;
  };

  template<typename ObjectClass, typename PropertyType>
  template<typename Signal>
  inline IntPropertySpin<ObjectClass, PropertyType>::IntPropertySpin(
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _object(object),
    _getter(getter),
    _setter(setter),
    _undoStack(undoStack),
    _skipUpdate(false)
  {
    connect(this,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &IntPropertySpin::updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &IntPropertySpin::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template<typename ObjectClass, typename PropertyType>
  inline void IntPropertySpin<ObjectClass, PropertyType>::setMaximum(int max)
  {
    QSpinBox::setMaximum(max);
    updateWidget();
  }

  template<typename ObjectClass, typename PropertyType>
  inline void IntPropertySpin<ObjectClass, PropertyType>::setMinimum(int min)
  {
    QSpinBox::setMinimum(min);
    updateWidget();
  }

  template<typename ObjectClass, typename PropertyType>
  inline void IntPropertySpin<ObjectClass, PropertyType>::
                                                      updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);
  
    try
    {
      PropertyType newValue = static_cast<PropertyType>(value());
      if ((_object.*_getter)() == newValue) return;

      using Command = SetPropertyCommand< ObjectClass,
                                          PropertyType,
                                          Setter>;
      std::unique_ptr<Command> command(new Command( _object,
                                                    _setter,
                                                    (_object.*_getter)(),
                                                    newValue));
      _undoStack.addAndMake(std::move(command));
    }
    catch(...)
    {
      Abort("IntPropertySpin::updateProperty: unable to update property.");
    }
  }

  template<typename ObjectClass, typename PropertyType>
  inline void IntPropertySpin<ObjectClass, PropertyType>::
                                                        updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      setValue(static_cast<int>((_object.*_getter)()));
    }
    catch (...)
    {
      Abort("IntPropertySpin::updateWidget: unable to update widget.");
    }
  }
}