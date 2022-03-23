#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3SpinConnection.h>

#include <Objects/Fluid/FluidObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class FluidWidget;
}

class FluidWidget : public QWidget
{
public:
  FluidWidget(FluidObject& object, mtt::UndoStack& undoStack);
  FluidWidget(const FluidWidget&) = delete;
  FluidWidget& operator = (const FluidWidget&) = delete;
  virtual ~FluidWidget() noexcept;

private:
  std::unique_ptr<Ui::FluidWidget> _ui;

  using FloatConnection = mtt::FloatSpinConnection<FluidObject>;
  std::optional<FloatConnection> _cellSizeConnection;

  using Vec3Connection = mtt::Vec3SpinConnection<FluidObject>;
  std::optional<Vec3Connection> _windConnection;
};