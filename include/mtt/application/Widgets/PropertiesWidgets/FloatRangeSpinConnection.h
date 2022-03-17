#pragma once

#include <stdexcept>

#include <QtWidgets/QDoubleSpinBox>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/Range.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class FloatRangeSpinConnection : public QObject
  {
  public:
    using Getter = const Range<float>& (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(const Range<float>&);

  public:
    template<typename Signal>
    inline FloatRangeSpinConnection(QDoubleSpinBox& minWidget,
                                    QDoubleSpinBox& maxWidget,
                                    ObjectClass& object,
                                    Getter getter,
                                    Setter setter,
                                    Signal signal,
                                    UndoStack& undoStack);
    FloatRangeSpinConnection(const FloatRangeSpinConnection&) = delete;
    FloatRangeSpinConnection& operator =
                                    (const FloatRangeSpinConnection&) = delete;
    virtual ~FloatRangeSpinConnection() noexcept = default;

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
  inline FloatRangeSpinConnection<ObjectClass>::FloatRangeSpinConnection(
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
            &FloatRangeSpinConnection::_updateFromWidgets,
            Qt::DirectConnection);

    connect(&_maxWidget,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &FloatRangeSpinConnection::_updateFromWidgets,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &FloatRangeSpinConnection::_updateFromProperty,
            Qt::DirectConnection);

    _updateFromProperty();
  }

  template<typename ObjectClass>
  inline void
            FloatRangeSpinConnection<ObjectClass>::_updateFromWidgets() noexcept
  {
    if(_skipUpdate) return;
    if(_multiplier == 0.f) return;
    ScopedTrueSetter skipper(_skipUpdate);

    _updateMinmax();

    try
    {
      Range<float> newValue = Range<float>( _minWidget.value(),
                                            _maxWidget.value()) / _multiplier;
      if ((_object.*_getter)() == newValue) return;

      _undoStack.addAndMake(makeSetPropertyCommand( _object,
                                                    _getter,
                                                    _setter,
                                                    newValue));
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "FloatRangeSpinConnection::_updateFromWidgets: unable to update property.";
    }
    catch(...)
    {
      Log() << "FloatRangeSpinConnection::_updateFromWidgets: unable to update property.";
    }
  }

  template<typename ObjectClass>
  inline float
              FloatRangeSpinConnection<ObjectClass>::multiplier() const noexcept
  {
    return _multiplier;
  }

  template<typename ObjectClass>
  inline void FloatRangeSpinConnection<ObjectClass>::setMultiplier(
                                                        float newValue) noexcept
  {
    _multiplier = newValue;
    _updateFromProperty();
  }

  template<typename ObjectClass>
  inline void
          FloatRangeSpinConnection<ObjectClass>::_updateFromProperty() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      Range<float> value = (_object.*_getter)() * _multiplier;
      _minWidget.setMaximum(FLT_MAX);
      _maxWidget.setMinimum(-FLT_MAX);
      _minWidget.setValue(value.min());
      _maxWidget.setValue(value.max());
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "FloatRangeSpinConnection::_updateFromProperty: unable to update widget.";
    }
    catch (...)
    {
      Log() << "FloatRangeSpinConnection::_updateFromProperty: unable to update widget.";
    }
    _updateMinmax();
  }

  template<typename ObjectClass>
  inline void FloatRangeSpinConnection<ObjectClass>::_updateMinmax() noexcept
  {
    _minWidget.setMaximum(_maxWidget.value());
    _maxWidget.setMinimum(_minWidget.value());
  }
}