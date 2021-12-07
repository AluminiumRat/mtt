#pragma once

#include <QtWidgets/QCheckBox>

#include <mtt/Application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>

namespace mtt
{
  template <typename ObjectClass>
  class BoolPropertyWidget : public QCheckBox
  {
  public:
    using Getter = bool(ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(bool);

  public:
    template<typename Signal>
    inline BoolPropertyWidget(ObjectClass& object,
                              Getter getter,
                              Setter setter,
                              Signal signal,
                              UndoStack& undoStack);
    BoolPropertyWidget(const BoolPropertyWidget&) = delete;
    BoolPropertyWidget& operator = (const BoolPropertyWidget&) = delete;
    virtual ~BoolPropertyWidget() noexcept = default;
  
  private:
    using Connection = BoolCheckboxConnection<ObjectClass>;
    Connection _connection;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline BoolPropertyWidget<ObjectClass>::BoolPropertyWidget(
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _connection(*this, object, getter, setter, signal, undoStack)
  {
  }
}