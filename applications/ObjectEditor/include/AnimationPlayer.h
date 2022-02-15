#pragma once

#include <chrono>

#include <QtCore/QObject>

#include <mtt/editorLib/Objects/AnimationObject.h>

class EditorScene;

class AnimationPlayer : public QObject
{
  Q_OBJECT

public:
  using TimeType = mtt::AnimationObject::TimeType;

public:
  AnimationPlayer();
  AnimationPlayer(const AnimationPlayer&) = delete;
  AnimationPlayer& operator = (const AnimationPlayer&) = delete;
  virtual ~AnimationPlayer() noexcept;

  inline mtt::AnimationObject* currentAnimation() const noexcept;

public slots:
  void start(mtt::AnimationObject& animation) noexcept;
  void stop() noexcept;

signals:
  void animationStarted(mtt::AnimationObject& animation);
  void animationStopped(mtt::AnimationObject& animation);

private:
  void _playNextFrame() noexcept;

private:
  mtt::AnimationObject* _currentAnimation;
  std::unique_ptr<mtt::AbstractEditCommand> _restoreCommand;

  QTimer _playTimer;
  std::chrono::system_clock::time_point _startTime;
};

inline mtt::AnimationObject* AnimationPlayer::currentAnimation() const noexcept
{
  return _currentAnimation;
}
