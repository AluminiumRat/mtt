#pragma once

#include <chrono>

#include <QtCore/QObject>

#include <mtt/editorLib/Objects/AnimationObject.h>

namespace mtt
{
  class AnimationPlayer : public QObject
  {
    Q_OBJECT

  public:
    using TimeType = AnimationObject::TimeType;

  public:
    AnimationPlayer();
    AnimationPlayer(const AnimationPlayer&) = delete;
    AnimationPlayer& operator = (const AnimationPlayer&) = delete;
    virtual ~AnimationPlayer() noexcept;

    inline AnimationObject* currentAnimation() const noexcept;

  public slots:
    void start(AnimationObject& animation) noexcept;
    void stop() noexcept;

  signals:
    void animationStarted(AnimationObject& animation);
    void animationStopped(AnimationObject& animation);

  private:
    void _playNextFrame() noexcept;

  private:
    AnimationObject* _currentAnimation;
    std::unique_ptr<AbstractEditCommand> _restoreCommand;

    QTimer _playTimer;
    std::chrono::system_clock::time_point _startTime;
  };

  inline AnimationObject* AnimationPlayer::currentAnimation() const noexcept
  {
    return _currentAnimation;
  }
}