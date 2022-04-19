#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/GasEmissionAction.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class GasEmissionActionWidget;
}

class GasEmissionActionWidget : public QWidget
{
public:
  GasEmissionActionWidget(GasEmissionAction& object,
                          mtt::Object& targetSelectArea,
                          mtt::UndoStack& undoStack);
  GasEmissionActionWidget(const GasEmissionActionWidget&) = delete;
  GasEmissionActionWidget& operator = (
                                const GasEmissionActionWidget&) = delete;
  virtual ~GasEmissionActionWidget() noexcept;

private:
  std::unique_ptr<Ui::GasEmissionActionWidget> _ui;

  using FloatConnection = mtt::FloatSpinConnection<GasEmissionAction>;
  std::optional<FloatConnection> _volumeConnection;
};