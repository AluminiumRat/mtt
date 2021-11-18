#pragma once

#include <mtt/Core/EditCommands/SetPropertyCommand.h>
#include <mtt/Core/EditCommands/UndoStack.h>
#include <mtt/Core/Widgets/PropertiesWidgets/ColorWidget.h>
#include <mtt/Utilities/Abort.h>
#include <mtt/Utilities/ScopedSetter.h>

namespace mtt
{
  template <typename ObjectClass>
  class ColorPropertyWidget : public ColorWidget
  {
  public:
    using Getter = const glm::vec3& (ObjectClass::*)() const noexcept;
    using Setter = void (ObjectClass::*)(const glm::vec3&);

  public:
    template<typename Signal>
    inline ColorPropertyWidget( ObjectClass& object,
                                Getter getter,
                                Setter setter,
                                Signal signal,
                                UndoStack& undoStack);
    ColorPropertyWidget(const ColorPropertyWidget&) = delete;
    ColorPropertyWidget& operator = (const ColorPropertyWidget&) = delete;
    virtual ~ColorPropertyWidget() noexcept = default;
  
  protected:
    inline virtual void onColorChanged(
                                  const glm::vec3& newValue) noexcept override;

  private:
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
  inline ColorPropertyWidget<ObjectClass>::ColorPropertyWidget(
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
    connect(&_object,
            signal,
            this,
            &ColorPropertyWidget::updateWidget,
            Qt::DirectConnection);

    updateWidget();
  }

  template<typename ObjectClass>
  inline void ColorPropertyWidget<ObjectClass>::onColorChanged(
                                            const glm::vec3& newValue) noexcept
  {
    ColorWidget::onColorChanged(newValue);
  
    if(_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);

    try
    {
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
      Abort("ColorPropertyWidget::onColorChanged: unable to update property.");
    }
  }

  template<typename ObjectClass>
  inline void ColorPropertyWidget<ObjectClass>::updateWidget() noexcept
  {
    if (_skipUpdate) return;
    ScopedTrueSetter skipper(_skipUpdate);
    setColor((_object.*_getter)());
  }
}