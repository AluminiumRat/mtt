#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/editorLib/Objects/DisplayedObject.h>

namespace Ui
{
  class VisiblePropertyWidget;
}

namespace mtt
{
  class UndoStack;

  class VisiblePropertyWidget : public QWidget
  {
  public:
    VisiblePropertyWidget(DisplayedObject& object,
                          UndoStack& undoStack);
    VisiblePropertyWidget(const VisiblePropertyWidget&) = delete;
    VisiblePropertyWidget& operator = (const VisiblePropertyWidget&) = delete;
    virtual ~VisiblePropertyWidget() noexcept;

  private:
    std::unique_ptr<Ui::VisiblePropertyWidget> _ui;
    using VisibleConnection = BoolCheckboxConnection<DisplayedObject>;
    std::optional<VisibleConnection> _visibleConnection;
  };
}