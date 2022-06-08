#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/IntSpinConnection.h>
#include <mtt/editorLib/Objects/SpotLightObject.h>

namespace Ui
{
  class SpotLightWidget;
}

namespace mtt
{
  class UndoStack;

  class SpotLightWidget : public QWidget
  {
  public:
    SpotLightWidget(SpotLightObject& object, UndoStack& undoStack);
    SpotLightWidget(const SpotLightWidget&) = delete;
    SpotLightWidget& operator = (const SpotLightWidget&) = delete;
    virtual ~SpotLightWidget() noexcept;

  private:
    std::unique_ptr<Ui::SpotLightWidget> _ui;

    using FloatConnection = FloatSpinConnection<SpotLightObject>;
    std::optional<FloatConnection> _angleConnection;

    using BoolConnection = BoolCheckboxConnection<SpotLightObject>;
    std::optional<BoolConnection> _shadowsConnection;

    std::optional<FloatConnection> _blurConnection;

    using IntConnection = IntSpinConnection<SpotLightObject, size_t>;
    std::optional<IntConnection> _mapSizeConnection;
  };
}