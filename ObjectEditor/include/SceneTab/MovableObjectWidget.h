#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <Objects/MovableObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class MovableObjectWidget;
}

class MovableObjectWidget : public QWidget
{
public:
  MovableObjectWidget(MovableObject& object, mtt::UndoStack& undoStack);
  MovableObjectWidget(const MovableObjectWidget&) = delete;
  MovableObjectWidget& operator = (const MovableObjectWidget&) = delete;
  virtual ~MovableObjectWidget() noexcept;

private:
  std::unique_ptr<Ui::MovableObjectWidget> _ui;
};
