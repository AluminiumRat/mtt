#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/Application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>

#include <Objects/DisplayedObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class VisiblePropertyWidget;
}

class VisiblePropertyWidget : public QWidget
{
public:
  VisiblePropertyWidget(DisplayedObject& object, mtt::UndoStack& undoStack);
  VisiblePropertyWidget(const VisiblePropertyWidget&) = delete;
  VisiblePropertyWidget& operator = (const VisiblePropertyWidget&) = delete;
  virtual ~VisiblePropertyWidget() noexcept;

private:
  std::unique_ptr<Ui::VisiblePropertyWidget> _ui;
  using VisibleConnection = mtt::BoolCheckboxConnection<DisplayedObject>;
  std::optional<VisibleConnection> _visibleConnection;
};
