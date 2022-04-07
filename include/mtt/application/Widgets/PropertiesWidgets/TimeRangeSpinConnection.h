#pragma once

#include <chrono>
#include <stdexcept>

#include <QtWidgets/QDoubleSpinBox>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/TimeT.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/Range.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class TimeRangeSpinConnection : public QObject
  {
  public:
    using Getter = const Range<TimeT>& (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(const Range<TimeT>&);

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

    inline void updateFromProperty() noexcept;

  private:
    inline void _updateFromMinWidget() noexcept;
    inline void _updateFromMaxWidget() noexcept;

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
            &TimeRangeSpinConnection::_updateFromMinWidget,
            Qt::DirectConnection);

    connect(&_maxWidget,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &TimeRangeSpinConnection::_updateFromMaxWidget,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &TimeRangeSpinConnection::updateFromProperty,
            Qt::DirectConnection);

    updateFromProperty();
  }

  template<typename ObjectClass>
  inline void
          TimeRangeSpinConnection<ObjectClass>::_updateFromMinWidget() noexcept
  {
    if(_skipUpdate) return;
    if(_multiplier == 0.f) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      Range<TimeT> oldValue = (_object.*_getter)();

      float minTime = _minWidget.value() / _multiplier;
      float maxTime = toFloatTime(oldValue.max());
      if(maxTime < minTime) maxTime = minTime;
      Range<TimeT> newValue = Range<TimeT>(toTimeT(minTime), toTimeT(maxTime));

      if (oldValue == newValue) return;

      _undoStack.addAndMake(makeSetPropertyCommand( _object,
                                                    _getter,
                                                    _setter,
                                                    newValue));

      _maxWidget.setValue(maxTime * _multiplier);
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "TimeRangeSpinConnection::_updateFromMinWidget: unable to update property.";
    }
    catch(...)
    {
      Log() << "TimeRangeSpinConnection::_updateFromMinWidget: unable to update property.";
    }
  }

  template<typename ObjectClass>
  inline void
          TimeRangeSpinConnection<ObjectClass>::_updateFromMaxWidget() noexcept
  {
    if(_skipUpdate) return;
    if(_multiplier == 0.f) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      Range<TimeT> oldValue = (_object.*_getter)();

      float minTime = toFloatTime(oldValue.min());
      float maxTime = _maxWidget.value() / _multiplier;

      if(minTime > maxTime) minTime = maxTime;

      Range<TimeT> newValue = Range<TimeT>(toTimeT(minTime), toTimeT(maxTime));
      if (oldValue == newValue) return;

      _undoStack.addAndMake(makeSetPropertyCommand( _object,
                                                    _getter,
                                                    _setter,
                                                    newValue));

      _minWidget.setValue(minTime * _multiplier);
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "TimeRangeSpinConnection::_updateFromMaxWidget: unable to update property.";
    }
    catch(...)
    {
      Log() << "TimeRangeSpinConnection::_updateFromMaxWidget: unable to update property.";
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
    updateFromProperty();
  }

  template<typename ObjectClass>
  inline void
            TimeRangeSpinConnection<ObjectClass>::updateFromProperty() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      Range<TimeT> value = (_object.*_getter)();
      _minWidget.setValue(toFloatTime(value.min()) * _multiplier);
      _maxWidget.setValue(toFloatTime(value.max()) * _multiplier);
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
  }
}