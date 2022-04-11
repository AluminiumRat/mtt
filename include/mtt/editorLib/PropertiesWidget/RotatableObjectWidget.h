#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <mtt/editorLib/Objects/RotatableObject.h>

namespace Ui
{
  class RotatableObjectWidget;
}

namespace mtt
{
  class UndoStack;

  class RotatableObjectWidget : public QWidget
  {
  public:
    RotatableObjectWidget(RotatableObject& object,
                          UndoStack& undoStack);
    RotatableObjectWidget(const RotatableObjectWidget&) = delete;
    RotatableObjectWidget& operator = (const RotatableObjectWidget&) = delete;
    virtual ~RotatableObjectWidget() noexcept;

  private:
    std::unique_ptr<Ui::RotatableObjectWidget> _ui;
  };
}