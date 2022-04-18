#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>

namespace Ui
{
  class PositionAnimatorWidget;
}

namespace mtt
{
  class PositionAnimator;
  class UndoStack;

  class PositionAnimatorWidget : public QWidget
  {
  public:
    PositionAnimatorWidget( PositionAnimator& object,
                            Object& skeletonSelectArea,
                            UndoStack& undoStack);
    PositionAnimatorWidget(const PositionAnimatorWidget&) = delete;
    PositionAnimatorWidget& operator = (const PositionAnimatorWidget&) = delete;
    virtual ~PositionAnimatorWidget() noexcept;

  private:
    std::unique_ptr<Ui::PositionAnimatorWidget> _ui;
  };
}
