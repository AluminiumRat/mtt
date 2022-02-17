#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <mtt/editorLib/Objects/MovableObject.h>

namespace Ui
{
  class MovableObjectWidget;
}

namespace mtt
{
  class UndoStack;

  class MovableObjectWidget : public QWidget
  {
  public:
    MovableObjectWidget(MovableObject& object, UndoStack& undoStack);
    MovableObjectWidget(const MovableObjectWidget&) = delete;
    MovableObjectWidget& operator = (const MovableObjectWidget&) = delete;
    virtual ~MovableObjectWidget() noexcept;

  private:
    std::unique_ptr<Ui::MovableObjectWidget> _ui;
  };
}