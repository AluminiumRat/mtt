#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/IntSpinConnection.h>
#include <mtt/editorLib/Objects/DirectLightObject.h>

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
  DirectLightWidget(mtt::DirectLightObject& object, mtt::UndoStack& undoStack);
  DirectLightWidget(const DirectLightWidget&) = delete;
  DirectLightWidget& operator = (const DirectLightWidget&) = delete;
  virtual ~DirectLightWidget() noexcept;

private:
  std::unique_ptr<Ui::DirectLightWidget> _ui;

  using BoolConnection = mtt::BoolCheckboxConnection<mtt::DirectLightObject>;
  std::optional<BoolConnection> _shadowsConnection;

  using FloatConnection = mtt::FloatSpinConnection<mtt::DirectLightObject>;
  std::optional<FloatConnection> _radiusConnection;
  std::optional<FloatConnection> _blurConnection;

  using IntConnection = mtt::IntSpinConnection<mtt::DirectLightObject, size_t>;
  std::optional<IntConnection> _mapSizeConnection;
  std::optional<IntConnection> _cascadeSizeConnection;
};
