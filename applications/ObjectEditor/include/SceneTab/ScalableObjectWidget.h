#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <mtt/editorLib/Objects/ScalableObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class ScalableObjectWidget;
}

class ScalableObjectWidget : public QWidget
{
public:
  ScalableObjectWidget(mtt::ScalableObject& object, mtt::UndoStack& undoStack);
  ScalableObjectWidget(const ScalableObjectWidget&) = delete;
  ScalableObjectWidget& operator = (const ScalableObjectWidget&) = delete;
  virtual ~ScalableObjectWidget() noexcept;

private:
  std::unique_ptr<Ui::ScalableObjectWidget> _ui;
};
