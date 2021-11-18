#pragma once

#include <glm/common.hpp>

#include <QtWidgets/QSlider>

#include <mtt/Core/EditCommands/SetPropertyCommand.h>
#include <mtt/Core/EditCommands/UndoStack.h>
#include <mtt/Utilities/Abort.h>
#include <mtt/Utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class FloatPropertyLinearSlider : public QSlider
  {
  public:
    using Getter = float (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(float);

  public:
    template<typename Signal>
    inline FloatPropertyLinearSlider( ObjectClass& object,
                                      Getter getter,
                                      Setter setter,
                                      Signal signal,
                                      UndoStack& undoStack);
    FloatPropertyLinearSlider(const FloatPropertyLinearSlider&) = delete;
    FloatPropertyLinearSlider& operator = (
                                    const FloatPropertyLinearSlider&) = delete;
    virtual ~FloatPropertyLinearSlider() noexcept = default;
  
    inline float minValue() const noexcept;
    inline float maxValue() const noexcept;
    inline void setRange(float minValue, float maxValue) noexcept;

  protected:
    inline virtual void mousePressEvent(QMouseEvent* theEvent) override;
    inline virtual void mouseReleaseEvent(QMouseEvent* theEvent) override;

  private:
    inline void updateProperty() noexcept;
    inline void updateWidget() noexcept;
  
  private:
    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    float _minValue;
    float _maxValue;

    UndoStack& _undoStack;
    UndoStack::GroupLocker _groupCommandLocker;

    bool _skipUpdate;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline FloatPropertyLinearSlider<ObjectClass>::FloatPropertyLinearSlider(
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _object(object),
    _getter(getter),
    _setter(setter),
    _minValue(0),
    _maxValue(1),
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
            &FloatPropertyLinearSlider::updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &FloatPropertyLinearSlider::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template<typename ObjectClass>
  inline float FloatPropertyLinearSlider<ObjectClass>::minValue() const noexcept
  {
    return _minValue;
  }

  template<typename ObjectClass>
  inline float FloatPropertyLinearSlider<ObjectClass>::maxValue() const noexcept
  {
    return _maxValue;
  }

  template<typename ObjectClass>
  inline void FloatPropertyLinearSlider<ObjectClass>::setRange(
                                                        float minValue,
                                                        float maxValue) noexcept
  {
    if(_minValue == minValue && _maxValue == maxValue) return;
    _minValue = minValue;
    _maxValue = maxValue;
    updateWidget();
  }

  template<typename ObjectClass>
  inline void FloatPropertyLinearSlider<ObjectClass>::updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);
  
    try
    {
      float factor = float(value() - minimum()) / (maximum() - minimum());
      float newValue = glm::mix(_minValue, _maxValue, factor);

      if ((_object.*_getter)() == newValue) return;

      using Command = SetPropertyCommand< ObjectClass,
                                          float,
                                          Setter>;
      std::unique_ptr<Command> command(new Command( _object,
                                                    _setter,
                                                    (_object.*_getter)(),
                                                    newValue));
      _undoStack.addAndMake(std::move(command));
    }
    catch(...)
    {
      Abort("FloatPropertyLinearSlider::updateProperty: unable to update property.");
    }
  }

  template<typename ObjectClass>
  inline void FloatPropertyLinearSlider<ObjectClass>::updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    float factor = ((_object.*_getter)() - _minValue) / (_maxValue - _minValue);
    int newValue = minimum() + round((maximum() - minimum()) * factor);

    try
    {
      setValue(newValue);
    }
    catch (...)
    {
      Abort("FloatPropertyLinearSlider::updateWidget: unable to update widget.");
    }
  }

  template<typename ObjectClass>
  inline void FloatPropertyLinearSlider<ObjectClass>::mousePressEvent(
                                                          QMouseEvent* theEvent)
  {
    try
    {
      _groupCommandLocker.reset(_undoStack);
    }
    catch (...)
    {
      Abort("FloatPropertyLinearSlider::mouseReleaseEvent: unable to lock undo group");
    }

    QSlider::mousePressEvent(theEvent);
  }

  template<typename ObjectClass>
  inline void FloatPropertyLinearSlider<ObjectClass>::mouseReleaseEvent(
                                                          QMouseEvent* theEvent)
  {
    QSlider::mouseReleaseEvent(theEvent);
    _groupCommandLocker.reset();
  }
}