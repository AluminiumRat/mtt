#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/TimeSpinConnection.h>

#include <Objects/ActionAnimationTrack.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class ActionAnimationTrackWidget;
}

class ActionAnimationTrackWidget : public QWidget
{
public:
  ActionAnimationTrackWidget( ActionAnimationTrack& object,
                              mtt::UndoStack& undoStack);
  ActionAnimationTrackWidget(const ActionAnimationTrackWidget&) = delete;
  ActionAnimationTrackWidget& operator = (
                                    const ActionAnimationTrackWidget&) = delete;
  virtual ~ActionAnimationTrackWidget() noexcept;

private:
  std::unique_ptr<Ui::ActionAnimationTrackWidget> _ui;

  using FloatConnection = mtt::TimeSpinConnection<ActionAnimationTrack>;
  std::optional<FloatConnection> _startTimeConnection;
  std::optional<FloatConnection> _durationConnection;
};