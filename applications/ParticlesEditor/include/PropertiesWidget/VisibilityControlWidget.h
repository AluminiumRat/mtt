#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatRangeSpinConnection.h>

#include <Objects/VisibilityControlObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class VisibilityControlWidget;
}

class VisibilityControlWidget : public QWidget
{
public:
  VisibilityControlWidget(VisibilityControlObject& object,
                          mtt::UndoStack& undoStack);
  VisibilityControlWidget(const VisibilityControlWidget&) = delete;
  VisibilityControlWidget& operator = (const VisibilityControlWidget&) = delete;
  virtual ~VisibilityControlWidget() noexcept;

private:
  std::unique_ptr<Ui::VisibilityControlWidget> _ui;

  using RangeConnection =
                        mtt::FloatRangeSpinConnection<VisibilityControlObject>;
  std::optional<RangeConnection> _saturationConnection;
};