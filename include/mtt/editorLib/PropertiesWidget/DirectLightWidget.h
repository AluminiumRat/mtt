#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/IntSpinConnection.h>
#include <mtt/editorLib/Objects/DirectLightObject.h>

namespace Ui
{
  class DirectLightWidget;
}

namespace mtt
{
  class UndoStack;

  class DirectLightWidget : public QWidget
  {
  public:
    DirectLightWidget(DirectLightObject& object,
                      UndoStack& undoStack);
    DirectLightWidget(const DirectLightWidget&) = delete;
    DirectLightWidget& operator = (const DirectLightWidget&) = delete;
    virtual ~DirectLightWidget() noexcept;

  private:
    std::unique_ptr<Ui::DirectLightWidget> _ui;

    using BoolConnection = BoolCheckboxConnection<DirectLightObject>;
    std::optional<BoolConnection> _shadowsConnection;

    using FloatConnection = FloatSpinConnection<DirectLightObject>;
    std::optional<FloatConnection> _radiusConnection;
    std::optional<FloatConnection> _blurConnection;

    using IntConnection = IntSpinConnection<DirectLightObject, size_t>;
    std::optional<IntConnection> _mapSizeConnection;
    std::optional<IntConnection> _cascadeSizeConnection;
  };
}