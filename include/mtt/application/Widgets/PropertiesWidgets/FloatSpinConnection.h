#pragma once

#include <stdexcept>

#include <QtWidgets/QDoubleSpinBox>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class FloatSpinConnection : public QObject
  {
  public:
    using Getter = float (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(float);

  public:
    template<typename Signal>
    inline FloatSpinConnection( QDoubleSpinBox& widget,
                                ObjectClass& object,
                                Getter getter,
                                Setter setter,
                                Signal signal,
                                UndoStack& undoStack);
    FloatSpinConnection(const FloatSpinConnection&) = delete;
    FloatSpinConnection& operator = (const FloatSpinConnection&) = delete;
    virtual ~FloatSpinConnection() noexcept = default;

    inline float multiplier() const noexcept;
    inline void setMultiplier(float newValue) noexcept;

    inline void updateWidget() noexcept;

  private:
    inline void _updateProperty() noexcept;
  
  private:
    QDoubleSpinBox& _widget;
    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    float _multiplier;

    UndoStack& _undoStack;
    bool _skipUpdate;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline FloatSpinConnection<ObjectClass>::FloatSpinConnection(
                                                        QDoubleSpinBox& widget,
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _widget(widget),
    _object(object),
    _getter(getter),
    _setter(setter),
    _multiplier(1.f),
    _undoStack(undoStack),
    _skipUpdate(false)
  {
    connect(&_widget,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &FloatSpinConnection::_updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &FloatSpinConnection::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template<typename ObjectClass>
  inline void FloatSpinConnection<ObjectClass>::_updateProperty() noexcept
  {
    if(_skipUpdate) return;
    if(_multiplier == 0.f) return;
    ScopedSetter<bool> skipper(_skipUpdate, true);

    try
    {
      float newValue = _widget.value() / _multiplier;
      if ((_object.*_getter)() == newValue) return;

      _undoStack.addAndMake(std::move(makeSetPropertyCommand( _object,
                                                              _getter,
                                                              _setter,
                                                              newValue)));
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "FloatSpinConnection::_updateProperty: unable to update property.";
    }
    catch(...)
    {
      Log() << "FloatSpinConnection::_updateProperty: unable to update property.";
    }
  }

  template<typename ObjectClass>
  inline float FloatSpinConnection<ObjectClass>::multiplier() const noexcept
  {
    return _multiplier;
  }

  template<typename ObjectClass>
  inline void
        FloatSpinConnection<ObjectClass>::setMultiplier(float newValue) noexcept
  {
    _multiplier = newValue;
    updateWidget();
  }

  template<typename ObjectClass>
  inline void FloatSpinConnection<ObjectClass>::updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedSetter<bool> skipper(_skipUpdate, true);

    try
    {
      _widget.setValue((_object.*_getter)() * _multiplier);
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "FloatSpinConnection::updateWidget: unable to update widget.";
    }
    catch (...)
    {
      Log() << "FloatSpinConnection::updateWidget: unable to update widget.";
    }
  }
}