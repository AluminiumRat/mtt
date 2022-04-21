#pragma once

#include <stdexcept>

#include <glm/vec3.hpp>

#include <QtWidgets/QDoubleSpinBox>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class Vec3SpinConnection : public QObject
  {
  public:
    using Getter = const glm::vec3& (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(const glm::vec3&);

  public:
    template<typename Signal>
    inline Vec3SpinConnection(QDoubleSpinBox& xWidget,
                              QDoubleSpinBox& yWidget,
                              QDoubleSpinBox& zWidget,
                              ObjectClass& object,
                              Getter getter,
                              Setter setter,
                              Signal signal,
                              UndoStack& undoStack);
    Vec3SpinConnection(const Vec3SpinConnection&) = delete;
    Vec3SpinConnection& operator = (const Vec3SpinConnection&) = delete;
    virtual ~Vec3SpinConnection() noexcept = default;

    inline float multiplier() const noexcept;
    inline void setMultiplier(float newValue) noexcept;

    inline void updateWidgets() noexcept;

  private:
    inline void _updateProperty() noexcept;
  
  private:
    QDoubleSpinBox& _xWidget;
    QDoubleSpinBox& _yWidget;
    QDoubleSpinBox& _zWidget;
    ObjectClass& _object;
    Getter _getter;
    Setter _setter;

    float _multiplier;

    UndoStack& _undoStack;
    bool _skipUpdate;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline Vec3SpinConnection<ObjectClass>::Vec3SpinConnection(
                                                        QDoubleSpinBox& xWidget,
                                                        QDoubleSpinBox& yWidget,
                                                        QDoubleSpinBox& zWidget,
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _xWidget(xWidget),
    _yWidget(yWidget),
    _zWidget(zWidget),
    _object(object),
    _getter(getter),
    _setter(setter),
    _multiplier(1.f),
    _undoStack(undoStack),
    _skipUpdate(false)
  {
    connect(&_xWidget,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &Vec3SpinConnection::_updateProperty,
            Qt::DirectConnection);

    connect(&_yWidget,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &Vec3SpinConnection::_updateProperty,
            Qt::DirectConnection);

    connect(&_zWidget,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &Vec3SpinConnection::_updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &Vec3SpinConnection::updateWidgets,
            Qt::DirectConnection);

    updateWidgets();
  }

  template<typename ObjectClass>
  inline void Vec3SpinConnection<ObjectClass>::_updateProperty() noexcept
  {
    if(_skipUpdate) return;
    if(_multiplier == 0.f) return;
    ScopedSetter<bool> skipper(_skipUpdate, true);

    try
    {
      glm::vec3 newValue( _xWidget.value() / _multiplier,
                          _yWidget.value() / _multiplier,
                          _zWidget.value() / _multiplier);
      if ((_object.*_getter)() == newValue) return;

      _undoStack.addAndMake(std::move(makeSetPropertyCommand( _object,
                                                              _getter,
                                                              _setter,
                                                              newValue)));
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "Vec3SpinConnection::_updateProperty: unable to update property.";
    }
    catch(...)
    {
      Log() << "Vec3SpinConnection::_updateProperty: unable to update property.";
    }
  }

  template<typename ObjectClass>
  inline float Vec3SpinConnection<ObjectClass>::multiplier() const noexcept
  {
    return _multiplier;
  }

  template<typename ObjectClass>
  inline void
        Vec3SpinConnection<ObjectClass>::setMultiplier(float newValue) noexcept
  {
    _multiplier = newValue;
    updateWidgets();
  }

  template<typename ObjectClass>
  inline void Vec3SpinConnection<ObjectClass>::updateWidgets() noexcept
  {
    if (_skipUpdate) return;
    ScopedSetter<bool> skipper(_skipUpdate, true);

    try
    {
      glm::vec3 value = (_object.*_getter)();
      value *= _multiplier;
      _xWidget.setValue(value.x);
      _yWidget.setValue(value.y);
      _zWidget.setValue(value.z);
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Log() << "Vec3SpinConnection::updateWidgets: unable to update widgets.";
    }
    catch (...)
    {
      Log() << "Vec3SpinConnection::updateWidgets: unable to update widgets.";
    }
  }
}