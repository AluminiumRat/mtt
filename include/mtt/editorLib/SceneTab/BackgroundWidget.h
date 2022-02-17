#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>

namespace Ui
{
  class BackgroundWidget;
}

namespace mtt
{
  class UndoStack;

  class BackgroundWidget : public QWidget
  {
  public:
    BackgroundWidget(BackgroundObject& object, UndoStack& undoStack);
    BackgroundWidget(const BackgroundWidget&) = delete;
    BackgroundWidget& operator = (const BackgroundWidget&) = delete;
    virtual ~BackgroundWidget() noexcept;

  private:
    std::unique_ptr<Ui::BackgroundWidget> _ui;

    using BoolConnection = BoolCheckboxConnection<BackgroundObject>;
    std::optional<BoolConnection> _lightConnection;

    using FloatConnection = FloatSpinConnection<BackgroundObject>;
    std::optional<FloatConnection> _luminanceConnection;
    std::optional<FloatConnection> _startConnection;
    std::optional<FloatConnection> _lengthConnection;
  };
}
