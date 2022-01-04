#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/Application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/Application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/Application/Widgets/PropertiesWidgets/IntSpinConnection.h>

#include <Objects/DirectLightObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class DirectLightWidget;
}

class DirectLightWidget : public QWidget
{
public:
  DirectLightWidget(DirectLightObject& object, mtt::UndoStack& undoStack);
  DirectLightWidget(const DirectLightWidget&) = delete;
  DirectLightWidget& operator = (const DirectLightWidget&) = delete;
  virtual ~DirectLightWidget() noexcept;

private:
  std::unique_ptr<Ui::DirectLightWidget> _ui;

  using BoolConnection = mtt::BoolCheckboxConnection<DirectLightObject>;
  std::optional<BoolConnection> _shadowsConnection;

  using FloatConnection = mtt::FloatSpinConnection<DirectLightObject>;
  std::optional<FloatConnection> _radiusConnection;
  std::optional<FloatConnection> _blurConnection;

  using IntConnection = mtt::IntSpinConnection<DirectLightObject, size_t>;
  std::optional<IntConnection> _mapSizeConnection;
  std::optional<IntConnection> _cascadeSizeConnection;
};
