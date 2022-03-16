#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/TimeRangeSpinConnection.h>

#include <Objects/ParticleAnimation.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class ParticleAnimationWidget;
}

class ParticleAnimationWidget : public QWidget
{
public:
  ParticleAnimationWidget(ParticleAnimation& object, mtt::UndoStack& undoStack);
  ParticleAnimationWidget(const ParticleAnimationWidget&) = delete;
  ParticleAnimationWidget& operator = (const ParticleAnimationWidget&) = delete;
  virtual ~ParticleAnimationWidget() noexcept;

private:
  std::unique_ptr<Ui::ParticleAnimationWidget> _ui;

  using TimeRangeConnection = mtt::TimeRangeSpinConnection<ParticleAnimation>;
  std::optional<TimeRangeConnection> _timeRangeConnection;
};