#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <mtt/editorLib/Objects/ScalableObject.h>

namespace Ui
{
  class ScalableObjectWidget;
}

namespace mtt
{
  class UndoStack;

  class ScalableObjectWidget : public QWidget
  {
  public:
    ScalableObjectWidget(ScalableObject& object, UndoStack& undoStack);
    ScalableObjectWidget(const ScalableObjectWidget&) = delete;
    ScalableObjectWidget& operator = (const ScalableObjectWidget&) = delete;
    virtual ~ScalableObjectWidget() noexcept;

  private:
    std::unique_ptr<Ui::ScalableObjectWidget> _ui;
  };
}