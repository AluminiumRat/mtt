#pragma once

#include <QtWidgets/QDoubleSpinBox>

#include <mtt/Core/EditCommands/SetPropertyCommand.h>
#include <mtt/Core/EditCommands/UndoStack.h>
#include <mtt/Utilities/Abort.h>
#include <mtt/Utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class FloatPropertySpin : public QDoubleSpinBox
  {
  public:
    using Getter = float (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(float);

  public:
    template<typename Signal>
    inline FloatPropertySpin( ObjectClass& object,
                              Getter getter,
                              Setter setter,
                              Signal signal,
                              UndoStack& undoStack);
    FloatPropertySpin(const FloatPropertySpin&) = delete;
    FloatPropertySpin& operator = (const FloatPropertySpin&) = delete;
    virtual ~FloatPropertySpin() noexcept = default;
  
    inline void setMaximum(double max);
    inline void setMinimum(double min);

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

  template<typename ObjectClass>
  template<typename Signal>
  inline FloatPropertySpin<ObjectClass>::FloatPropertySpin(
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
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &FloatPropertySpin::updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &FloatPropertySpin::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template<typename ObjectClass>
  inline void FloatPropertySpin<ObjectClass>::setMaximum(double max)
  {
    QDoubleSpinBox::setMaximum(max);
    updateWidget();
  }

  template<typename ObjectClass>
  inline void FloatPropertySpin<ObjectClass>::setMinimum(double min)
  {
    QDoubleSpinBox::setMinimum(min);
    updateWidget();
  }

  template<typename ObjectClass>
  inline void FloatPropertySpin<ObjectClass>::updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);
  
    try
    {
      if ((_object.*_getter)() == value()) return;

      using Command = SetPropertyCommand< ObjectClass,
                                          float,
                                          Setter>;
      std::unique_ptr<Command> command(new Command( _object,
                                                    _setter,
                                                    (_object.*_getter)(),
                                                    value()));
      _undoStack.addAndMake(std::move(command));
    }
    catch(...)
    {
      Abort("FloatPropertySpin::updateProperty: unable to update property.");
    }
  }

  template<typename ObjectClass>
  inline void FloatPropertySpin<ObjectClass>::updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      setValue((_object.*_getter)());
    }
    catch (...)
    {
      Abort("FloatPropertySpin::updateWidget: unable to update widget.");
    }
  }
}