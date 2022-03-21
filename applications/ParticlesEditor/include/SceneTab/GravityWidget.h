#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/GravityModificator.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class GravityWidget;
}

class GravityWidget : public QWidget
{
public:
  GravityWidget(GravityModificator& object, mtt::UndoStack& undoStack);
  GravityWidget(const GravityWidget&) = delete;
  GravityWidget& operator = (const GravityWidget&) = delete;
  virtual ~GravityWidget() noexcept;

private:
  std::unique_ptr<Ui::GravityWidget> _ui;

  using FloatConnection = mtt::FloatSpinConnection<GravityModificator>;
  std::optional<FloatConnection> _accelerationConnection;
};