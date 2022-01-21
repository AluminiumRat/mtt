#pragma once

#include <glm/vec3.hpp>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3Widget.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class Vec3PropertyWidget : public Vec3Widget
  {
  public:
    using Getter = const glm::vec3& (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(const glm::vec3&);

  public:
    template<typename Signal>
    inline Vec3PropertyWidget(ObjectClass& object,
                              Getter getter,
                              Setter setter,
                              Signal signal,
                              UndoStack& undoStack);
    Vec3PropertyWidget(const Vec3PropertyWidget&) = delete;
    Vec3PropertyWidget& operator = (const Vec3PropertyWidget&) = delete;
    virtual ~Vec3PropertyWidget() noexcept = default;
  
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
  inline Vec3PropertyWidget<ObjectClass>::Vec3PropertyWidget(
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
            &Vec3PropertyWidget::updateProperty,
            Qt::DirectConnection);

    connect(&ySpin(),
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &Vec3PropertyWidget::updateProperty,
            Qt::DirectConnection);

    connect(&zSpin(),
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &Vec3PropertyWidget::updateProperty,
            Qt::DirectConnection);

    connect(&_object,
            signal,
            this,
            &Vec3PropertyWidget::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template<typename ObjectClass>
  inline void Vec3PropertyWidget<ObjectClass>::updateProperty() noexcept
  {
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);
  
    try
    {
      glm::vec3 newValue( xSpin().value(),
                          ySpin().value(),
                          zSpin().value());
      if ((_object.*_getter)() == newValue) return;

      using Command = SetPropertyCommand< ObjectClass,
                                          glm::vec3,
                                          Setter>;
      std::unique_ptr<Command> command(new Command( _object,
                                                    _setter,
                                                    (_object.*_getter)(),
                                                    newValue));
      _undoStack.addAndMake(std::move(command));
    }
    catch(...)
    {
      Abort("Vec3PropertyWidget::updateProperty: unable to update property.");
    }
  }

  template<typename ObjectClass>
  inline void Vec3PropertyWidget<ObjectClass>::updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
      glm::vec3 value = (_object.*_getter)();
      xSpin().setValue(value.x);
      ySpin().setValue(value.y);
      zSpin().setValue(value.z);
    }
    catch (...)
    {
      Abort("Vec3PropertyWidget::updateWidget: unable to update widget.");
    }
  }
}