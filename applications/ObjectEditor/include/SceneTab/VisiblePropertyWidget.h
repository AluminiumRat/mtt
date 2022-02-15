#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/editorLib/Objects/DisplayedObject.h>

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
  VisiblePropertyWidget(mtt::DisplayedObject& object,
                        mtt::UndoStack& undoStack);
  VisiblePropertyWidget(const VisiblePropertyWidget&) = delete;
  VisiblePropertyWidget& operator = (const VisiblePropertyWidget&) = delete;
  virtual ~VisiblePropertyWidget() noexcept;

private:
  std::unique_ptr<Ui::VisiblePropertyWidget> _ui;
  using VisibleConnection = mtt::BoolCheckboxConnection<mtt::DisplayedObject>;
  std::optional<VisibleConnection> _visibleConnection;
};
