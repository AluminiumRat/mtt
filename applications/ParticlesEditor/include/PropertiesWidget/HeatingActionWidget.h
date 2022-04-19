#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/HeatingAction.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class HeatingActionWidget;
}

class HeatingActionWidget : public QWidget
{
public:
  HeatingActionWidget(HeatingAction& object,
                      mtt::Object& targetSelectArea,
                      mtt::UndoStack& undoStack);
  HeatingActionWidget(const HeatingActionWidget&) = delete;
  HeatingActionWidget& operator = (const HeatingActionWidget&) = delete;
  virtual ~HeatingActionWidget() noexcept;

private:
  std::unique_ptr<Ui::HeatingActionWidget> _ui;

  using FloatConnection = mtt::FloatSpinConnection<HeatingAction>;
  std::optional<FloatConnection> _energyConnection;
};