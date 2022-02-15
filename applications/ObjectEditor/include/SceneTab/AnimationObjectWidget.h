#pragma once

#include <memory>

#include <QtWidgets/QWidget>

namespace mtt
{
  class AnimationObject;
}

namespace Ui
{
  class AnimationObjectWidget;
}

class AnimationPlayer;

class AnimationObjectWidget : public QWidget
{
public:
  AnimationObjectWidget(mtt::AnimationObject& object,
                        AnimationPlayer& player);
  AnimationObjectWidget(const AnimationObjectWidget&) = delete;
  AnimationObjectWidget& operator = (const AnimationObjectWidget&) = delete;
  virtual ~AnimationObjectWidget() noexcept;

private:
  void _play() noexcept;
  void _stop() noexcept;

private:
  std::unique_ptr<Ui::AnimationObjectWidget> _ui;
  mtt::AnimationObject& _object;
  AnimationPlayer& _player;
};
