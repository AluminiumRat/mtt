#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class AnimationTrackWidget;
}

class AnimationTrackWidget : public QWidget
{
public:
  AnimationTrackWidget( mtt::AnimationTrack& object,
                        mtt::Object& skeletonSelectArea,
                        mtt::UndoStack& undoStack);
  AnimationTrackWidget(const AnimationTrackWidget&) = delete;
  AnimationTrackWidget& operator = (const AnimationTrackWidget&) = delete;
  virtual ~AnimationTrackWidget() noexcept;

private:
  std::unique_ptr<Ui::AnimationTrackWidget> _ui;
  using EnabledConnection = mtt::BoolCheckboxConnection<mtt::AnimationTrack>;
  std::optional<EnabledConnection> _enabledConnection;
};

