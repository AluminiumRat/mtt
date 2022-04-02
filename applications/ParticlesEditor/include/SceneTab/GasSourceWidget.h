#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/Fluid/GasSource.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class GasSourceWidget;
}

class GasSourceWidget : public QWidget
{
public:
  GasSourceWidget(GasSource& object, mtt::UndoStack& undoStack);
  GasSourceWidget(const GasSourceWidget&) = delete;
  GasSourceWidget& operator = (const GasSourceWidget&) = delete;
  virtual ~GasSourceWidget() noexcept;

private:
  std::unique_ptr<Ui::GasSourceWidget> _ui;

  using FloatConnection = mtt::FloatSpinConnection<GasSource>;
  std::optional<FloatConnection> _sizeConnection;
  std::optional<FloatConnection> _flowRateConnection;
  std::optional<FloatConnection> _temperatureConnection;
};