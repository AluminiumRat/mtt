#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/LightObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class LightWidget;
}

class LightWidget : public QWidget
{
public:
  LightWidget(LightObject& object, mtt::UndoStack& undoStack);
  LightWidget(const LightWidget&) = delete;
  LightWidget& operator = (const LightWidget&) = delete;
  virtual ~LightWidget() noexcept;

private:
  std::unique_ptr<Ui::LightWidget> _ui;

  using BoolConnection = mtt::BoolCheckboxConnection<LightObject>;
  std::optional<BoolConnection> _enabledConnection;

  using FloatConnection = mtt::FloatSpinConnection<LightObject>;
  std::optional<FloatConnection> _powerConnection;
  std::optional<FloatConnection> _distanceConnection;
};
