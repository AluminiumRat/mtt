#pragma once

#include <chrono>
#include <stdexcept>

#include <QtWidgets/QDoubleSpinBox>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/TimeT.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class TimeSpinConnection : public QObject
  {
  public:
    using Getter = TimeT (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(TimeT);

  public:
    template<typename Signal>
    inline TimeSpinConnection(QDoubleSpinBox& widget,
                              ObjectClass& object,
                              Getter getter,
                              Setter setter,
                              Signal signal,
                              UndoStack& undoStack);
    TimeSpinConnection(const TimeSpinConnection&) = delete;
    TimeSpinConnection& operator =
                                    (const TimeSpinConnection&) = delete;
    virtual ~TimeSpinConnection() noexcept = default;

    inline float multiplier() const noexcept;
    inline void setMultiplier(float newValue) noexcept;

    inline void _updateFromProperty() noexcept;

  private:
    inline void _updateFromWidget() noexcept;

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
  inline TimeSpinConnection<ObjectClass>::TimeSpinConnection(
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
            &TimeSpinConnection::_updateFromWidget,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &TimeSpinConnection::_updateFromProperty,
            Qt::DirectConnection);

    _updateFromProperty();
  }

  template<typename ObjectClass>
  inline void
            TimeSpinConnection<ObjectClass>::_updateFromWidget() noexcept
  {
    if(_skipUpdate) return;
    if(_multiplier == 0.f) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      using FloatTimeType = std::chrono::duration<float>;
      FloatTimeType floatTime(_widget.value() / _multiplier);
      TimeT newValue = std::chrono::duration_cast<TimeT>(floatTime);
      if ((_object.*_getter)() == newValue) return;

      _undoStack.addAndMake(makeSetPropertyCommand( _object,
                                                    _getter,
                                                    _setter,
                                                    newValue));
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "TimeSpinConnection::_updateFromWidgets: unable to update property.";
    }
    catch(...)
    {
      Log() << "TimeSpinConnection::_updateFromWidgets: unable to update property.";
    }
  }

  template<typename ObjectClass>
  inline float
              TimeSpinConnection<ObjectClass>::multiplier() const noexcept
  {
    return _multiplier;
  }

  template<typename ObjectClass>
  inline void TimeSpinConnection<ObjectClass>::setMultiplier(
                                                        float newValue) noexcept
  {
    _multiplier = newValue;
    _updateFromProperty();
  }

  template<typename ObjectClass>
  inline void
          TimeSpinConnection<ObjectClass>::_updateFromProperty() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      using FloatTimeType = std::chrono::duration<float>;
      FloatTimeType floatTime =
                std::chrono::duration_cast<FloatTimeType>((_object.*_getter)());

      _widget.setValue(floatTime.count() * _multiplier);
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "TimeSpinConnection::_updateFromProperty: unable to update widget.";
    }
    catch (...)
    {
      Log() << "TimeSpinConnection::_updateFromProperty: unable to update widget.";
    }
  }
}