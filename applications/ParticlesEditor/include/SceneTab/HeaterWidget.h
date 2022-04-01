#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/HeaterObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class HeaterWidget;
}

class HeaterWidget : public QWidget
{
public:
  HeaterWidget(HeaterObject& object, mtt::UndoStack& undoStack);
  HeaterWidget(const HeaterWidget&) = delete;
  HeaterWidget& operator = (const HeaterWidget&) = delete;
  virtual ~HeaterWidget() noexcept;

private:
  std::unique_ptr<Ui::HeaterWidget> _ui;

  using FloatConnection = mtt::FloatSpinConnection<HeaterObject>;
  std::optional<FloatConnection> _sizeConnection;
  std::optional<FloatConnection> _powerConnection;
};