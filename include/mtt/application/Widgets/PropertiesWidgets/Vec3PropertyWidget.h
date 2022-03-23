#pragma once

#include <mtt/application/Widgets/PropertiesWidgets/Vec3SpinConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3Widget.h>

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

    inline void updateWidget() noexcept;

  private:
    Vec3SpinConnection<ObjectClass> _connection;
  };

  template<typename ObjectClass>
  template<typename Signal>
  inline Vec3PropertyWidget<ObjectClass>::Vec3PropertyWidget(
                                                        ObjectClass& object,
                                                        Getter getter,
                                                        Setter setter,
                                                        Signal signal,
                                                        UndoStack& undoStack) :
    _connection(xSpin(),
                ySpin(),
                zSpin(),
                object,
                getter,
                setter,
                signal,
                undoStack)
  {
  }

  template<typename ObjectClass>
  inline void Vec3PropertyWidget<ObjectClass>::updateWidget() noexcept
  {
    _connection.updateWidgets();
  }
}