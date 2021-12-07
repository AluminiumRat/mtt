#pragma once

#include <memory>

#include <QtWidgets/QWidget>

namespace Ui
{
  class AnimationObjectWidget;
}

class AnimationObject;
class AnimationPlayer;

class AnimationObjectWidget : public QWidget
{
public:
  AnimationObjectWidget(AnimationObject& object,
                        AnimationPlayer& player);
  AnimationObjectWidget(const AnimationObjectWidget&) = delete;
  AnimationObjectWidget& operator = (const AnimationObjectWidget&) = delete;
  virtual ~AnimationObjectWidget() noexcept;

private:
  void _play() noexcept;
  void _stop() noexcept;

private:
  std::unique_ptr<Ui::AnimationObjectWidget> _ui;
  AnimationObject& _object;
  AnimationPlayer& _player;
};
