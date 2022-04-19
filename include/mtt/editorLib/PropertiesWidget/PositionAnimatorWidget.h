#pragma once

#include <memory>

#include <QtWidgets/QWidget>

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
