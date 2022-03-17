#pragma once

#include <map>
#include <stdexcept>
#include <vector>

#include <QtWidgets/QComboBox>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass, typename EnumType>
  class EnumComboConnection : public QObject
  {
  public:
    using Getter = EnumType (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(EnumType);
    using NameMap = std::map<EnumType, QString>;

  public:
    template<typename Signal>
    inline EnumComboConnection( QComboBox& widget,
                                ObjectClass& object,
                                Getter getter,
                                Setter setter,
                                Signal signal,
                                const NameMap& nameMap,
                                UndoStack& undoStack);
    EnumComboConnection(const EnumComboConnection&) = delete;
    EnumComboConnection& operator = (const EnumComboConnection&) = delete;
    virtual ~EnumComboConnection() noexcept = default;

    inline void updateWidget() noexcept;

  private:
    inline void _updateProperty() noexcept;

  private:
    QComboBox& _widget;
    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    UndoStack& _undoStack;
    bool _skipUpdate;

    std::vector<EnumType> _enumValues;
  };

  template <typename ObjectClass, typename EnumType>
  template<typename Signal>
  inline EnumComboConnection<ObjectClass, EnumType>::EnumComboConnection(
                                                        QComboBox& widget,
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        const NameMap& nameMap,
                                                        UndoStack& undoStack) :
    _widget(widget),
    _object(object),
    _getter(getter),
    _setter(setter),
    _undoStack(undoStack),
    _skipUpdate(false)
  {
    for(typename NameMap::const_iterator iNameMap = nameMap.begin();
        iNameMap != nameMap.end();
        iNameMap++)
    {
      widget.addItem(iNameMap->second);
      _enumValues.push_back(iNameMap->first);
    }

    connect(&_widget,
            QOverload<int>::of(&QComboBox::activated),
            this,
            &EnumComboConnection::_updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &EnumComboConnection::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template <typename ObjectClass, typename EnumType>
  inline void
          EnumComboConnection<ObjectClass, EnumType>::_updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      if(_widget.currentIndex() < 0) return;
      EnumType value = _enumValues[_widget.currentIndex()];
      if ((_object.*_getter)() == value) return;

      _undoStack.addAndMake(makeSetPropertyCommand( _object,
                                                    _getter,
                                                    _setter,
                                                    value));
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "EnumComboConnection::_updateProperty: unable to update property.";
    }
    catch(...)
    {
      Log() << "EnumComboConnection::_updateProperty: unable to update property.";
    }
  }

  template <typename ObjectClass, typename EnumType>
  inline void
            EnumComboConnection<ObjectClass, EnumType>::updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    int valueIndex = 0;
    for (; valueIndex < _enumValues.size(); valueIndex++)
    {
      if(_enumValues[valueIndex] == (_object.*_getter)()) break;
    }
    if (valueIndex == _enumValues.size())
    {
      Log() << "EnumComboConnection::updateWidget: unable to find enum value " << int((_object.*_getter)()) << " in name map";
      return;
    }

    try
    {
      _widget.setCurrentIndex(valueIndex);
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "EnumComboConnection::updateWidget: unable to update widget.";
    }
    catch (...)
    {
      Log() << "EnumComboConnection::updateWidget: unable to update widget.";
    }
  }
}