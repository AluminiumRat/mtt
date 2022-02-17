#include <stdexcept>

#include <mtt/editorLib/AnimationPlayer.h>

using namespace mtt;

AnimationPlayer::AnimationPlayer() :
  _currentAnimation(nullptr)
{
  _playTimer.setSingleShot(false);
  connect(&_playTimer,
          &QTimer::timeout,
          this,
          &AnimationPlayer::_playNextFrame,
          Qt::DirectConnection);
}

AnimationPlayer::~AnimationPlayer() noexcept
{
  if(_currentAnimation != nullptr) stop();
}

void AnimationPlayer::start(AnimationObject& animation) noexcept
{
  if(_currentAnimation != nullptr) stop();
  
  try
  {
    _restoreCommand = animation.makeRestoreCommand();
    _currentAnimation = &animation;

    _startTime = std::chrono::system_clock::now();
    _playTimer.start(0);
  }
  catch(std::exception& error)
  {
    _restoreCommand.reset();
    _currentAnimation = nullptr;
    Log() << error.what();
    Log() << "AnimationPlayer::start: unable to strt animation.";
  }
  catch(...)
  {
    _restoreCommand.reset();
    _currentAnimation = nullptr;
    Log() << "AnimationPlayer::start: unable to strt animation.";
  }
}

void AnimationPlayer::stop() noexcept
{
  if(_currentAnimation == nullptr) return;

  _playTimer.stop();
  
  AnimationObject* animation = _currentAnimation;
  _currentAnimation = nullptr;
  
  try
  {
    if(_restoreCommand != nullptr) _restoreCommand->make();
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("AnimationPlayer::stop: unable to restor initail state.");
  }
  catch(...)
  {
    Abort("AnimationPlayer::stop: unable to restor initail state.");
  }

  emit animationStopped(*animation);
}

void AnimationPlayer::_playNextFrame() noexcept
{
  if(_currentAnimation == nullptr) Abort("AnimationPlayer::_playNextFrame: current animation is null.");

  try
  {
    std::chrono::system_clock::time_point now =
                                              std::chrono::system_clock::now();
    std::chrono::system_clock::duration duration = now - _startTime;
    TimeType animationTime = std::chrono::duration_cast<TimeType>(duration);

    if(animationTime > _currentAnimation->finishTime()) stop();
    else _currentAnimation->update(animationTime);
  }
  catch(std::exception& error)
  {
    Log() << "AnimationObject::_playNextFrame: unable to play animation: " << error.what();
    stop();
  }
  catch(...)
  {
    Log() << "AnimationObject::_playNextFrame: unknown error in animation play";
    stop();
  }
}
