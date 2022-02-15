#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/editorLib/Objects/LightObject.h>

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
  LightWidget(mtt::LightObject& object, mtt::UndoStack& undoStack);
  LightWidget(const LightWidget&) = delete;
  LightWidget& operator = (const LightWidget&) = delete;
  virtual ~LightWidget() noexcept;

private:
  std::unique_ptr<Ui::LightWidget> _ui;

  using BoolConnection = mtt::BoolCheckboxConnection<mtt::LightObject>;
  std::optional<BoolConnection> _enabledConnection;

  using FloatConnection = mtt::FloatSpinConnection<mtt::LightObject>;
  std::optional<FloatConnection> _powerConnection;
  std::optional<FloatConnection> _distanceConnection;
};
