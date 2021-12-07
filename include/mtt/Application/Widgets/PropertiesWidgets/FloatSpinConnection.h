#pragma once

#include <stdexcept>

#include <QtWidgets/QDoubleSpinBox>

#include <mtt/Application/EditCommands/SetPropertyCommand.h>
#include <mtt/Application/EditCommands/UndoStack.h>
#include <mtt/Utilities/Log.h>
#include <mtt/Utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class FloatSpinConnection : public QObject
  {
  public:
    using Getter = float (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(float);

  public:
    template<typename Signal>
    inline FloatSpinConnection( QDoubleSpinBox& widget,
                                ObjectClass& object,
                                Getter getter,
                                Setter setter,
                                Signal signal,
                                UndoStack& undoStack);
    FloatSpinConnection(const FloatSpinConnection&) = delete;
    FloatSpinConnection& operator = (const FloatSpinConnection&) = delete;
    virtual ~FloatSpinConnection() noexcept = default;
  
    inline void updateWidget() noexcept;

  private:
    inline void _updateProperty() noexcept;
  
  private:
    QDoubleSpinBox& _widget;
    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    UndoStack& _undoStack;
    bool _skipUpdate;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline FloatSpinConnection<ObjectClass>::FloatSpinConnection(
                                                        QDoubleSpinBox& widget,
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _widget(widget),
    _object(object),
    _getter(getter),
    _setter(setter),
    _undoStack(undoStack),
    _skipUpdate(false)
  {
    connect(&_widget,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &FloatSpinConnection::_updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &FloatSpinConnection::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template<typename ObjectClass>
  inline void FloatSpinConnection<ObjectClass>::_updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);
  
    try
    {
      if ((_object.*_getter)() == _widget.value()) return;

      using Command = SetPropertyCommand< ObjectClass,
                                          float,
                                          Setter>;
      std::unique_ptr<Command> command(new Command( _object,
                                                    _setter,
                                                    (_object.*_getter)(),
                                                    _widget.value()));
      _undoStack.addAndMake(std::move(command));
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "FloatSpinConnection::_updateProperty: unable to update property.";
    }
    catch(...)
    {
      Log() << "FloatSpinConnection::_updateProperty: unable to update property.";
    }
  }

  template<typename ObjectClass>
  inline void FloatSpinConnection<ObjectClass>::updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      _widget.setValue((_object.*_getter)());
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "FloatSpinConnection::updateWidget: unable to update widget.";
    }
    catch (...)
    {
      Log() << "FloatSpinConnection::updateWidget: unable to update widget.";
    }
  }
}