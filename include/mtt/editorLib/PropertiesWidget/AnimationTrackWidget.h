#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>

namespace Ui
{
  class AnimationTrackWidget;
}

namespace mtt
{
  class UndoStack;

  class AnimationTrackWidget : public QWidget
  {
  public:
    AnimationTrackWidget( AnimationTrack& object,
                          Object& skeletonSelectArea,
                          UndoStack& undoStack);
    AnimationTrackWidget(const AnimationTrackWidget&) = delete;
    AnimationTrackWidget& operator = (const AnimationTrackWidget&) = delete;
    virtual ~AnimationTrackWidget() noexcept;

  private:
    std::unique_ptr<Ui::AnimationTrackWidget> _ui;
    using EnabledConnection = BoolCheckboxConnection<AnimationTrack>;
    std::optional<EnabledConnection> _enabledConnection;
  };
}
