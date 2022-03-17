#pragma once

#include <stdexcept>

#include <QtWidgets/QSpinBox>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass, typename PropertyType>
  class IntSpinConnection : public QObject
  {
  public:
    using Getter = PropertyType(ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(PropertyType);

  public:
    template<typename Signal>
    inline IntSpinConnection( QSpinBox& widget,
                              ObjectClass& object,
                              Getter getter,
                              Setter setter,
                              Signal signal,
                              UndoStack& undoStack);
    IntSpinConnection(const IntSpinConnection&) = delete;
    IntSpinConnection& operator = (const IntSpinConnection&) = delete;
    virtual ~IntSpinConnection() noexcept = default;
  
    inline void updateWidget() noexcept;

  private:
    inline void _updateProperty() noexcept;
  
  private:
    QSpinBox& _widget;

    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    UndoStack& _undoStack;
    bool _skipUpdate;
  };

  template<typename ObjectClass, typename PropertyType>
  template<typename Signal>
  inline IntSpinConnection<ObjectClass, PropertyType>::IntSpinConnection(
                                                        QSpinBox& widget,
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
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &IntSpinConnection::_updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &IntSpinConnection::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template<typename ObjectClass, typename PropertyType>
  inline void IntSpinConnection<ObjectClass, PropertyType>::
                                                    _updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);
  
    try
    {
      PropertyType newValue = static_cast<PropertyType>(_widget.value());
      if ((_object.*_getter)() == newValue) return;

      _undoStack.addAndMake(makeSetPropertyCommand( _object,
                                                    _getter,
                                                    _setter,
                                                    newValue));
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "IntSpinConnection::updateWidget: unable to update widget.";
    }
    catch(...)
    {
      Log() << "IntSpinConnection::_updateProperty: unable to update property.";
    }
  }

  template<typename ObjectClass, typename PropertyType>
  inline void IntSpinConnection<ObjectClass, PropertyType>::
                                                        updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      _widget.setValue(static_cast<int>((_object.*_getter)()));
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "IntSpinConnection::updateWidget: unable to update widget.";
    }
    catch (...)
    {
      Log() << "IntSpinConnection::updateWidget: unable to update widget.";
    }
  }
}