#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/IntSpinConnection.h>
#include <mtt/editorLib/Objects/LightWithShadowsObject.h>

namespace Ui
{
  class ShadowsWidget;
}

namespace mtt
{
  class UndoStack;

  class ShadowsWidget : public QWidget
  {
  public:
    ShadowsWidget(LightWithShadowsObject& object,
                  float blurScaleFactor,
                  float maxBlurValue,
                  UndoStack& undoStack);
    ShadowsWidget(const ShadowsWidget&) = delete;
    ShadowsWidget& operator = (const ShadowsWidget&) = delete;
    virtual ~ShadowsWidget() noexcept;

  private:
    std::unique_ptr<Ui::ShadowsWidget> _ui;

    using BoolConnection = BoolCheckboxConnection<LightWithShadowsObject>;
    std::optional<BoolConnection> _enabledConnection;

    using IntConnection = IntSpinConnection<LightWithShadowsObject, size_t>;
    std::optional<IntConnection> _mapSizeConnection;

    using FloatConnection = FloatSpinConnection<LightWithShadowsObject>;
    std::optional<FloatConnection> _blurConnection;
  };
}