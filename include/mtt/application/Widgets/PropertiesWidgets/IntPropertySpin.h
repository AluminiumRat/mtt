#pragma once

#include <QtWidgets/QSpinBox>

#include <mtt/application/Widgets/PropertiesWidgets/IntSpinConnection.h>

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
    IntSpinConnection<ObjectClass, PropertyType> _connection;
  };

  template<typename ObjectClass, typename PropertyType>
  template<typename Signal>
  inline IntPropertySpin<ObjectClass, PropertyType>::IntPropertySpin(
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _connection(*this, object, getter, setter, signal, undoStack)
  {
  }

  template<typename ObjectClass, typename PropertyType>
  inline void IntPropertySpin<ObjectClass, PropertyType>::setMaximum(int max)
  {
    QSpinBox::setMaximum(max);
    _connection.updateWidget();
  }

  template<typename ObjectClass, typename PropertyType>
  inline void IntPropertySpin<ObjectClass, PropertyType>::setMinimum(int min)
  {
    QSpinBox::setMinimum(min);
    _connection.updateWidget();
  }
}