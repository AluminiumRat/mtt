#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/editorLib/Objects/AmbientLightObject.h>

namespace Ui
{
  class AmbientLightWidget;
}

namespace mtt
{
  class UndoStack;

  class AmbientLightWidget : public QWidget
  {
  public:
    AmbientLightWidget( AmbientLightObject& object,
                        UndoStack& undoStack);
    AmbientLightWidget(const AmbientLightWidget&) = delete;
    AmbientLightWidget& operator = (const AmbientLightWidget&) = delete;
    virtual ~AmbientLightWidget() noexcept;

  private:
    std::unique_ptr<Ui::AmbientLightWidget> _ui;

    using SaturationConnection = FloatSpinConnection<AmbientLightObject>;
    std::optional<SaturationConnection> _saturationConnection;
  };
}
