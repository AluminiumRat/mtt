#pragma once

#include <stdexcept>

#include <QtWidgets/QCheckBox>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class BoolCheckboxConnection : public QObject
  {
  public:
    using Getter = bool(ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(bool);

  public:
    template<typename Signal>
    inline BoolCheckboxConnection(QCheckBox& checkbox,
                                  ObjectClass& object,
                                  Getter getter,
                                  Setter setter,
                                  Signal signal,
                                  UndoStack& undoStack);
    BoolCheckboxConnection(const BoolCheckboxConnection&) = delete;
    BoolCheckboxConnection& operator = (const BoolCheckboxConnection&) = delete;
    virtual ~BoolCheckboxConnection() noexcept = default;
  
  private:
    inline void _updateProperty() noexcept;
    inline void _updateWidget() noexcept;
  
  private:
    QCheckBox& _checkbox;

    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    UndoStack& _undoStack;
    bool _skipUpdate;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline BoolCheckboxConnection<ObjectClass>::BoolCheckboxConnection(
                                                        QCheckBox& checkbox,
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _checkbox(checkbox),
    _object(object),
    _getter(getter),
    _setter(setter),
    _undoStack(undoStack),
    _skipUpdate(false)
  {
    connect(&_checkbox,
            &QCheckBox::clicked,
            this,
            &BoolCheckboxConnection::_updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &BoolCheckboxConnection::_updateWidget,
            Qt::DirectConnection);

    _updateWidget();
  }

  template<typename ObjectClass>
  inline void BoolCheckboxConnection<ObjectClass>::_updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedSetter<bool> skipper(_skipUpdate, true);

    try
    {
      bool newValue = _checkbox.isChecked();
      if ((_object.*_getter)() == newValue) return;

      _undoStack.addAndMake(std::move(makeSetPropertyCommand( _object,
                                                              _getter,
                                                              _setter,
                                                              newValue)));
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "BoolCheckboxConnection::_updateProperty: unable to update property.";
    }
    catch(...)
    {
      Log() << "BoolCheckboxConnection::_updateProperty: unable to update property.";
    }
  }

  template<typename ObjectClass>
  inline void BoolCheckboxConnection<ObjectClass>::_updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedSetter<bool> skipper(_skipUpdate, true);

    try
    {
      _checkbox.setChecked((_object.*_getter)());
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "BoolCheckboxConnection::_updateWidget: unable to update widget.";
    }
    catch (...)
    {
      Log() << "BoolCheckboxConnection::_updateWidget: unable to update widget.";
    }
  }
}