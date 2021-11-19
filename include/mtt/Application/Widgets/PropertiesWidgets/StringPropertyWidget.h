#pragma once

#include <QtWidgets/QLineEdit>

#include <mtt/Application/EditCommands/SetPropertyCommand.h>
#include <mtt/Application/EditCommands/UndoStack.h>
#include <mtt/Utilities/Abort.h>
#include <mtt/Utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class StringPropertyWidget : public QLineEdit
  {
  public:
    using Getter = QString(ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(const QString&);

  public:
    template<typename Signal>
    inline StringPropertyWidget(ObjectClass& object,
                                Getter getter,
                                Setter setter,
                                Signal signal,
                                UndoStack& undoStack);
    StringPropertyWidget(const StringPropertyWidget&) = delete;
    StringPropertyWidget& operator = (const StringPropertyWidget&) = delete;
    virtual ~StringPropertyWidget() noexcept = default;
  
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
  inline StringPropertyWidget<ObjectClass>::StringPropertyWidget(
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
            &QLineEdit::editingFinished,
            this,
            &StringPropertyWidget::updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &StringPropertyWidget::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template<typename ObjectClass>
  inline void StringPropertyWidget<ObjectClass>::updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      QString newValue = text();
      if ((_object.*_getter)() == newValue) return;

      using Command = SetPropertyCommand< ObjectClass,
                                          QString,
                                          Setter>;
      std::unique_ptr<Command> command(new Command( _object,
                                                    _setter,
                                                    (_object.*_getter)(),
                                                    newValue));
      _undoStack.addAndMake(std::move(command));
    }
    catch(...)
    {
      Abort("StringPropertyWidget::updateProperty: unable to update property.");
    }
  }

  template<typename ObjectClass>
  inline void StringPropertyWidget<ObjectClass>::updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      setText((_object.*_getter)());
    }
    catch (...)
    {
      Abort("StringPropertyWidget::updateWidget: unable to update widget.");
    }
  }
}