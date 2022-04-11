#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/editorLib/Objects/LightObject.h>

namespace Ui
{
  class LightWidget;
}

namespace mtt
{
  class UndoStack;

  class LightWidget : public QWidget
  {
  public:
    LightWidget(LightObject& object, UndoStack& undoStack);
    LightWidget(const LightWidget&) = delete;
    LightWidget& operator = (const LightWidget&) = delete;
    virtual ~LightWidget() noexcept;

  private:
    std::unique_ptr<Ui::LightWidget> _ui;

    using BoolConnection = BoolCheckboxConnection<LightObject>;
    std::optional<BoolConnection> _enabledConnection;

    using FloatConnection = FloatSpinConnection<LightObject>;
    std::optional<FloatConnection> _powerConnection;
    std::optional<FloatConnection> _distanceConnection;
  };
}