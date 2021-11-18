#pragma once

#include <QtWidgets/QCheckBox>

#include <mtt/Core/EditCommands/SetPropertyCommand.h>
#include <mtt/Core/EditCommands/UndoStack.h>
#include <mtt/Utilities/Abort.h>
#include <mtt/Utilities/ScopedSetter.h>

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
    inline void updateProperty() noexcept;
    inline void updateWidget() noexcept;
  
  private:
    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    UndoStack& _undoStack;
    bool _skipUpdate;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline BoolPropertyWidget<ObjectClass>::BoolPropertyWidget(
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _object(object),
    _getter(getter),
    _setter(setter),
    _undoStack(undoStack),
    _skipUpdate(false)
  {
    connect(this,
            &QCheckBox::clicked,
            this,
            &BoolPropertyWidget::updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &BoolPropertyWidget::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template<typename ObjectClass>
  inline void BoolPropertyWidget<ObjectClass>::updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      bool newValue = isChecked();
      if ((_object.*_getter)() == newValue) return;

      using Command = SetPropertyCommand< ObjectClass,
                                          bool,
                                          Setter>;
      std::unique_ptr<Command> command(new Command( _object,
                                                    _setter,
                                                    (_object.*_getter)(),
                                                    newValue));
      _undoStack.addAndMake(std::move(command));
    }
    catch(...)
    {
      Abort("BoolPropertyWidget::updateProperty: unable to update property.");
    }
  }

  template<typename ObjectClass>
  inline void BoolPropertyWidget<ObjectClass>::updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      setChecked((_object.*_getter)());
    }
    catch (...)
    {
      Abort("BoolPropertyWidget::updateWidget: unable to update widget.");
    }
  }
}