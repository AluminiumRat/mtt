#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <Objects/RotatableObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class RotatableObjectWidget;
}

class RotatableObjectWidget : public QWidget
{
public:
  RotatableObjectWidget(RotatableObject& object, mtt::UndoStack& undoStack);
  RotatableObjectWidget(const RotatableObjectWidget&) = delete;
  RotatableObjectWidget& operator = (const RotatableObjectWidget&) = delete;
  virtual ~RotatableObjectWidget() noexcept;

private:
  std::unique_ptr<Ui::RotatableObjectWidget> _ui;
};
