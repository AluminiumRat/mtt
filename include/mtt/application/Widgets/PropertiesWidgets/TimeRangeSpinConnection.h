#pragma once

#include <chrono>
#include <stdexcept>

#include <QtWidgets/QDoubleSpinBox>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Application.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/Range.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class TimeRangeSpinConnection : public QObject
  {
  public:
    using TimeType = Application::TimeType;
    using Getter = const Range<TimeType>& (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(const Range<TimeType>&);

  public:
    template<typename Signal>
    inline TimeRangeSpinConnection( QDoubleSpinBox& minWidget,
                                    QDoubleSpinBox& maxWidget,
                                    ObjectClass& object,
                                    Getter getter,
                                    Setter setter,
                                    Signal signal,
                                    UndoStack& undoStack);
    TimeRangeSpinConnection(const TimeRangeSpinConnection&) = delete;
    TimeRangeSpinConnection& operator =
                                    (const TimeRangeSpinConnection&) = delete;
    virtual ~TimeRangeSpinConnection() noexcept = default;

    inline float multiplier() const noexcept;
    inline void setMultiplier(float newValue) noexcept;

    inline void _updateFromProperty() noexcept;

  private:
    inline void _updateFromWidgets() noexcept;
    inline void _updateMinmax() noexcept;

  private:
    QDoubleSpinBox& _minWidget;
    QDoubleSpinBox& _maxWidget;
    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    float _multiplier;

    UndoStack& _undoStack;
    bool _skipUpdate;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline TimeRangeSpinConnection<ObjectClass>::TimeRangeSpinConnection(
                                                      QDoubleSpinBox& minWidget,
                                                      QDoubleSpinBox& maxWidget,
                                                      ObjectClass& object,
                                                      Getter getter,
                                                      Setter setter,
                                                      Signal signal,
                                                      UndoStack& undoStack) :
    _minWidget(minWidget),
    _maxWidget(maxWidget),
    _object(object),
    _getter(getter),
    _setter(setter),
    _multiplier(1.f),
    _undoStack(undoStack),
    _skipUpdate(false)
  {
    connect(&_minWidget,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &TimeRangeSpinConnection::_updateFromWidgets,
            Qt::DirectConnection);

    connect(&_maxWidget,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &TimeRangeSpinConnection::_updateFromWidgets,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &TimeRangeSpinConnection::_updateFromProperty,
            Qt::DirectConnection);

    _updateFromProperty();
  }

  template<typename ObjectClass>
  inline void
            TimeRangeSpinConnection<ObjectClass>::_updateFromWidgets() noexcept
  {
    if(_skipUpdate) return;
    if(_multiplier == 0.f) return;
    ScopedTrueSetter skipper(_skipUpdate);

    _updateMinmax();

    try
    {
      using FloatTimeType = std::chrono::duration<float>;
      FloatTimeType minTime(_minWidget.value() / _multiplier);
      FloatTimeType maxTime(_maxWidget.value() / _multiplier);
      Range<TimeType> newValue = Range<TimeType>(
                                std::chrono::duration_cast<TimeType>(minTime),
                                std::chrono::duration_cast<TimeType>(maxTime));
      if ((_object.*_getter)() == newValue) return;

      using Command = SetPropertyCommand< ObjectClass,
                                          Range<TimeType>,
                                          Setter>;
      std::unique_ptr<Command> command(new Command( _object,
                                                    _setter,
                                                    (_object.*_getter)(),
                                                    newValue));
      _undoStack.addAndMake(std::move(command));
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "TimeRangeSpinConnection::_updateFromWidgets: unable to update property.";
    }
    catch(...)
    {
      Log() << "TimeRangeSpinConnection::_updateFromWidgets: unable to update property.";
    }
  }

  template<typename ObjectClass>
  inline float
              TimeRangeSpinConnection<ObjectClass>::multiplier() const noexcept
  {
    return _multiplier;
  }

  template<typename ObjectClass>
  inline void TimeRangeSpinConnection<ObjectClass>::setMultiplier(
                                                        float newValue) noexcept
  {
    _multiplier = newValue;
    _updateFromProperty();
  }

  template<typename ObjectClass>
  inline void
          TimeRangeSpinConnection<ObjectClass>::_updateFromProperty() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      Range<TimeType> value = (_object.*_getter)();
      using FloatTimeType = std::chrono::duration<float>;
      FloatTimeType minTime(
                        std::chrono::duration_cast<FloatTimeType>(value.min()));
      FloatTimeType maxTime(
                        std::chrono::duration_cast<FloatTimeType>(value.max()));

      _minWidget.setMaximum(FLT_MAX);
      _maxWidget.setMinimum(-FLT_MAX);
      _minWidget.setValue(minTime.count() * _multiplier);
      _maxWidget.setValue(maxTime.count() * _multiplier);
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "TimeRangeSpinConnection::_updateFromProperty: unable to update widget.";
    }
    catch (...)
    {
      Log() << "TimeRangeSpinConnection::_updateFromProperty: unable to update widget.";
    }
    _updateMinmax();
  }

  template<typename ObjectClass>
  inline void TimeRangeSpinConnection<ObjectClass>::_updateMinmax() noexcept
  {
    _minWidget.setMaximum(_maxWidget.value());
    _maxWidget.setMinimum(_minWidget.value());
  }
}