#pragma once

#include <glm/common.hpp>

#include <QtWidgets/QSlider>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class FloatPropertyProgressiveSlider : public QSlider
  {
  public:
    using Getter = float (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(float);

  public:
    template<typename Signal>
    inline FloatPropertyProgressiveSlider(ObjectClass& object,
                                          Getter getter,
                                          Setter setter,
                                          Signal signal,
                                          UndoStack& undoStack);
    FloatPropertyProgressiveSlider(
                                const FloatPropertyProgressiveSlider&) = delete;
    FloatPropertyProgressiveSlider& operator = (
                                const FloatPropertyProgressiveSlider&) = delete;
    virtual ~FloatPropertyProgressiveSlider() noexcept = default;
  
    inline float minValue() const noexcept;
    inline float maxValue() const noexcept;
    inline void setRange(float minValue, float maxValue) noexcept;

    inline float doublingSteps() const noexcept;
    inline float setDoublingSteps(float newValue) noexcept;

  protected:
    inline virtual void mousePressEvent(QMouseEvent* theEvent) override;
    inline virtual void mouseReleaseEvent(QMouseEvent* theEvent) override;

  private:
    inline void _updateProperty() noexcept;
    inline void _updateWidget() noexcept;
  
  private:
    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    float _minValue;
    float _maxValue;
    float _doublingSteps;

    UndoStack& _undoStack;
    UndoStack::GroupLocker _groupCommandLocker;

    bool _skipUpdate;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline FloatPropertyProgressiveSlider<ObjectClass>::
                        FloatPropertyProgressiveSlider( ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _object(object),
    _getter(getter),
    _setter(setter),
    _minValue(0),
    _maxValue(1),
    _doublingSteps(10),
    _undoStack(undoStack),
    _skipUpdate(false)
  {
    setMinimum(0);
    setMaximum(1000);
    setSingleStep(10);
    setPageStep(100);

    setOrientation(Qt::Horizontal);

    connect(this,
            &QSlider::valueChanged,
            this,
            &FloatPropertyProgressiveSlider::_updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &FloatPropertyProgressiveSlider::_updateWidget,
            Qt::DirectConnection);

    _updateWidget();
  }

  template<typename ObjectClass>
  inline float FloatPropertyProgressiveSlider<ObjectClass>::
                                                      minValue() const noexcept
  {
    return _minValue;
  }

  template<typename ObjectClass>
  inline float FloatPropertyProgressiveSlider<ObjectClass>::
                                                      maxValue() const noexcept
  {
    return _maxValue;
  }

  template<typename ObjectClass>
  inline void FloatPropertyProgressiveSlider<ObjectClass>::setRange(
                                                        float minValue,
                                                        float maxValue) noexcept
  {
    if(_minValue == minValue && _maxValue == maxValue) return;
    _minValue = minValue;
    _maxValue = maxValue;
    _updateWidget();
  }

  template<typename ObjectClass>
  inline float FloatPropertyProgressiveSlider<ObjectClass>::
                                                  doublingSteps() const noexcept
  {
    return _doublingSteps;
  }

  template<typename ObjectClass>
  inline float FloatPropertyProgressiveSlider<ObjectClass>::
                                      setDoublingSteps(float newValue) noexcept
  {
    if(_doublingSteps == newValue) return;
    _doublingSteps = newValue;
    _updateWidget();
  }

  template<typename ObjectClass>
  inline void FloatPropertyProgressiveSlider<ObjectClass>::
                                                      _updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedSetter<bool> skipper(_skipUpdate, true);
  
    try
    {
      float propertyFactor = 0;
      if(value() != minimum())
      {
        float widgetFactor =
                          float(value() - minimum()) / (maximum() - minimum());
        propertyFactor = pow(2.f, _doublingSteps * widgetFactor) /
                                                      pow(2.f, _doublingSteps);
      }
      
      float newValue = glm::mix(_minValue, _maxValue, propertyFactor);

      if ((_object.*_getter)() == newValue) return;

      _undoStack.addAndMake(makeSetPropertyCommand( _object,
                                                    _getter,
                                                    _setter,
                                                    newValue));
    }
    catch(...)
    {
      Log() << "FloatPropertyProgressiveSlider::updateProperty: unable to update property.";
    }
  }

  template<typename ObjectClass>
  inline void FloatPropertyProgressiveSlider<ObjectClass>::
                                                       _updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedSetter<bool> skipper(_skipUpdate, true);

    float widgetFactor = 0;
    if((_object.*_getter)() != _minValue)
    {
      float propertyFactor =
                  ((_object.*_getter)() - _minValue) / (_maxValue - _minValue);
      widgetFactor = log2(pow(2.f, _doublingSteps) * propertyFactor) /
                                                                _doublingSteps;
    }

    int newValue = minimum() + round((maximum() - minimum()) * widgetFactor);

    try
    {
      setValue(newValue);
    }
    catch (...)
    {
      Abort("FloatPropertyProgressiveSlider::updateWidget: unable to update widget.");
    }
  }

  template<typename ObjectClass>
  inline void FloatPropertyProgressiveSlider<ObjectClass>::mousePressEvent(
                                                          QMouseEvent* theEvent)
  {
    try
    {
      _groupCommandLocker.reset(_undoStack);
    }
    catch (...)
    {
      Abort("FloatPropertyProgressiveSlider::mouseReleaseEvent: unable to lock undo group");
    }

    QSlider::mousePressEvent(theEvent);
  }

  template<typename ObjectClass>
  inline void FloatPropertyProgressiveSlider<ObjectClass>::mouseReleaseEvent(
                                                          QMouseEvent* theEvent)
  {
    QSlider::mouseReleaseEvent(theEvent);
    _groupCommandLocker.reset();
  }
}