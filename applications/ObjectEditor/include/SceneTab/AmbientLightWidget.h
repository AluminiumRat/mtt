#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/Application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/AmbientLightObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class AmbientLightWidget;
}

class AmbientLightWidget : public QWidget
{
public:
  AmbientLightWidget( AmbientLightObject& object,
                      mtt::UndoStack& undoStack);
  AmbientLightWidget(const AmbientLightWidget&) = delete;
  AmbientLightWidget& operator = (const AmbientLightWidget&) = delete;
  virtual ~AmbientLightWidget() noexcept;

private:
  std::unique_ptr<Ui::AmbientLightWidget> _ui;
  using SaturationConnection = mtt::FloatSpinConnection<AmbientLightObject>;
  std::optional<SaturationConnection> _saturationConnection;
};
