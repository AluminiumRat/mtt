#pragma once

#include <stdexcept>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Widgets/PropertiesWidgets/Bitfield32Widget.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class Bitfield32PropertyConnection : public QObject
  {
  public:
    using Getter = uint32_t(ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(uint32_t);

  public:
    template<typename Signal>
    inline Bitfield32PropertyConnection(Bitfield32Widget& widget,
                                        ObjectClass& object,
                                        Getter getter,
                                        Setter setter,
                                        Signal signal,
                                        UndoStack& undoStack);
    Bitfield32PropertyConnection(const Bitfield32PropertyConnection&) = delete;
    Bitfield32PropertyConnection& operator = (
                                  const Bitfield32PropertyConnection&) = delete;
    virtual ~Bitfield32PropertyConnection() noexcept = default;

  private:
    inline void _updateProperty() noexcept;
    inline void _updateWidget() noexcept;

  private:
    Bitfield32Widget& _widget;

    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    UndoStack& _undoStack;
    bool _skipUpdate;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline Bitfield32PropertyConnection<ObjectClass>::
                                Bitfield32PropertyConnection(
                                                      Bitfield32Widget& widget,
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
            &Bitfield32Widget::valueChanged,
            this,
            &Bitfield32PropertyConnection::_updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &Bitfield32PropertyConnection::_updateWidget,
            Qt::DirectConnection);

    _updateWidget();
  }

  template<typename ObjectClass>
  inline void Bitfield32PropertyConnection<ObjectClass>::_updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      uint32_t newValue = _widget.value();
      if ((_object.*_getter)() == newValue) return;

      _undoStack.addAndMake(makeSetPropertyCommand( _object,
                                                    _getter,
                                                    _setter,
                                                    newValue));
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "Bitfield32PropertyConnection::_updateProperty: unable to update property.";
    }
    catch(...)
    {
      Log() << "Bitfield32PropertyConnection::_updateProperty: unable to update property.";
    }
  }

  template<typename ObjectClass>
  inline void Bitfield32PropertyConnection<ObjectClass>::_updateWidget() noexcept
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
      Log() << "Bitfield32PropertyConnection::_updateWidget: unable to update widget.";
    }
    catch (...)
    {
      Log() << "Bitfield32PropertyConnection::_updateWidget: unable to update widget.";
    }
  }
}