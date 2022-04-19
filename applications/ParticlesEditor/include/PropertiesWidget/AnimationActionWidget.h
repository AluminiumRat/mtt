#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/TimeSpinConnection.h>

#include <Objects/AnimationAction.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class AnimationActionWidget;
}

class AnimationActionWidget : public QWidget
{
public:
  AnimationActionWidget(AnimationAction& object,
                        mtt::UndoStack& undoStack);
  AnimationActionWidget(const AnimationActionWidget&) = delete;
  AnimationActionWidget& operator = (const AnimationActionWidget&) = delete;
  virtual ~AnimationActionWidget() noexcept;

private:
  std::unique_ptr<Ui::AnimationActionWidget> _ui;

  using FloatConnection = mtt::TimeSpinConnection<AnimationAction>;
  std::optional<FloatConnection> _startTimeConnection;
  std::optional<FloatConnection> _durationConnection;
};