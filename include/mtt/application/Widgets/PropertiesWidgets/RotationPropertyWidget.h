#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3Widget.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class RotationPropertyWidget : public Vec3Widget
  {
  public:
    using Getter = const glm::quat& (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(const glm::quat&);

  public:
    template<typename Signal>
    inline RotationPropertyWidget(ObjectClass& object,
                                  Getter getter,
                                  Setter setter,
                                  Signal signal,
                                  UndoStack& undoStack);
    RotationPropertyWidget(const RotationPropertyWidget&) = delete;
    RotationPropertyWidget& operator = (const RotationPropertyWidget&) = delete;
    virtual ~RotationPropertyWidget() noexcept = default;
  
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
  inline RotationPropertyWidget<ObjectClass>::RotationPropertyWidget(
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
    connect(&xSpin(),
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &RotationPropertyWidget::updateProperty,
            Qt::DirectConnection);

    connect(&ySpin(),
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &RotationPropertyWidget::updateProperty,
            Qt::DirectConnection);

    connect(&zSpin(),
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &RotationPropertyWidget::updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &RotationPropertyWidget::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template<typename ObjectClass>
  inline void RotationPropertyWidget<ObjectClass>::updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);
  
    try
    {
      glm::vec3 angles( glm::radians(xSpin().value()),
                        glm::radians(ySpin().value()),
                        glm::radians(zSpin().value()));
      glm::quat newValue(angles);
      if ((_object.*_getter)() == newValue) return;

      _undoStack.addAndMake(makeSetPropertyCommand( _object,
                                                    _getter,
                                                    _setter,
                                                    newValue));
    }
    catch(...)
    {
      Log() << "RotationPropertyWidget::updateProperty: unable to update property.";
    }
  }

  template<typename ObjectClass>
  inline void RotationPropertyWidget<ObjectClass>::updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      glm::vec3 angles = glm::eulerAngles((_object.*_getter)());
      xSpin().setValue(glm::degrees(angles.x));
      ySpin().setValue(glm::degrees(angles.y));
      zSpin().setValue(glm::degrees(angles.z));
    }
    catch (...)
    {
      Abort("RotationPropertyWidget::updateWidget: unable to update widget.");
    }
  }
}