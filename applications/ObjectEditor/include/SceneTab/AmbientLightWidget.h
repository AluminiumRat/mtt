#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>
#include <mtt/editorLib/Objects/AmbientLightObject.h>

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
  AmbientLightWidget( mtt::AmbientLightObject& object,
                      mtt::UndoStack& undoStack);
  AmbientLightWidget(const AmbientLightWidget&) = delete;
  AmbientLightWidget& operator = (const AmbientLightWidget&) = delete;
  virtual ~AmbientLightWidget() noexcept;

private:
  std::unique_ptr<Ui::AmbientLightWidget> _ui;
  using SaturationConnection =
                              mtt::FloatSpinConnection<mtt::AmbientLightObject>;
  std::optional<SaturationConnection> _saturationConnection;
};
