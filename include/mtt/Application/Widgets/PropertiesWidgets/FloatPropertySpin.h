#pragma once

#include <QtWidgets/QDoubleSpinBox>

#include <mtt/Application/EditCommands/SetPropertyCommand.h>
#include <mtt/Application/EditCommands/UndoStack.h>
#include <mtt/Application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
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
    FloatSpinConnection<ObjectClass> _connection;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline FloatPropertySpin<ObjectClass>::FloatPropertySpin(
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _connection(*this, object, getter, setter, signal, undoStack)
  {
  }

  template<typename ObjectClass>
  inline void FloatPropertySpin<ObjectClass>::setMaximum(double max)
  {
    QDoubleSpinBox::setMaximum(max);
    _connection.updateWidget();
  }

  template<typename ObjectClass>
  inline void FloatPropertySpin<ObjectClass>::setMinimum(double min)
  {
    QDoubleSpinBox::setMinimum(min);
    _connection.updateWidget();
  }
}