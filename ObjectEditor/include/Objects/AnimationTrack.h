#pragma once

#include <chrono>
#include <optional>
#include <memory>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <mtt/Animation/CompositeAnimatedTransform.h>
#include <mtt/Animation/KeypointsAnimatedValue.h>
#include <mtt/Animation/ValueKeypoint.h>
#include <mtt/Application/EditCommands/AbstractEditCommand.h>
#include <mtt/Application/Scene/ObjectLink.h>
#include <mtt/Application/Scene/Object.h>

#include <Objects/OEVisitorExtension.h>
#include <Objects/SkeletonObject.h>

class AnimationTrack : public mtt::Object
{
  Q_OBJECT

  Q_PROPERTY( bool enabled
              READ enabled
              WRITE setEnabled
              RESET resetEnabled
              NOTIFY enabledChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  using TimeType = std::chrono::duration<int32_t, std::ratio<1, 1000>>;
  using PositionKeypoint = mtt::ValueKeypoint<glm::vec3, TimeType>;
  using RotationKeypoint = mtt::ValueKeypoint<glm::quat, TimeType>;
  using ScaleKeypoint = mtt::ValueKeypoint<glm::vec3, TimeType>;

public:
  explicit AnimationTrack(const mtt::UID& id = mtt::UID());
  AnimationTrack(const AnimationTrack&) = delete;
  AnimationTrack& operator = (const AnimationTrack&) = delete;
  virtual ~AnimationTrack() noexcept = default;

  inline bool enabled() const noexcept;
  void setEnabled(bool newValue);
  inline void resetEnabled();

  void update(TimeType time);

  inline size_t positionKeypointNumber() const noexcept;
  inline const PositionKeypoint& positionKeypoint(size_t index) const noexcept;
  inline void addPositionKeypoint(std::unique_ptr<PositionKeypoint> keypoint);
  /// Returns removed keypoint
  inline std::unique_ptr<PositionKeypoint> removePositionKeypoint(
                                    const PositionKeypoint& keypoint) noexcept;

  inline size_t rotationKeypointNumber() const noexcept;
  inline const RotationKeypoint& rotationKeypoint(size_t index) const noexcept;
  inline void addRotationKeypoint(std::unique_ptr<RotationKeypoint> keypoint);
  /// Returns removed keypoint
  inline std::unique_ptr<RotationKeypoint> removeRotationKeypoint(
                                    const RotationKeypoint& keypoint) noexcept;

  inline size_t scaleKeypointNumber() const noexcept;
  inline const PositionKeypoint& scaleKeypoint(size_t index) const noexcept;
  inline void addScaleKeypoint(std::unique_ptr<ScaleKeypoint> keypoint);
  /// Returns removed keypoint
  inline std::unique_ptr<ScaleKeypoint> removeScaleKeypoint(
                                        const ScaleKeypoint& keypoint) noexcept;

  inline TimeType startTime() const noexcept;
  inline TimeType finishTime() const noexcept;
  inline TimeType duration() const noexcept;

  inline const mtt::ObjectRef<SkeletonObject>& skeletonRef() const noexcept;
  inline SkeletonObject* skeleton() const noexcept;
  /// You can use nullptr to remove link
  void setSkeleton(SkeletonObject* skeleton);
  /// You can use invalid UID to remove link
  void setSkeletonId(const mtt::UID& id);

  /// Makes a command to restore animated object after the animation has played.
  /// The command has no undo functional.
  std::unique_ptr<mtt::AbstractEditCommand> makeRestoreCommand() const;

signals:
  void enabledChanged(bool newValue);
  void startTimeChanged(TimeType newStartTime);
  void finishTimeChanged(TimeType newFinishTime);
  void durationChanged(TimeType newDuration);
  void timingChanged();
  void skeletonRefChanged(SkeletonObject* skeleton);

private:
  void _connectSkeleton(SkeletonObject& skeleton);
  void _disconnectSkeleton(SkeletonObject& skeleton) noexcept;

private:
  void _updateTiming() noexcept;

private:
  bool _enabled;

  using PositionAnimation = mtt::KeypointsAnimatedValue<PositionKeypoint>;
  PositionAnimation _positionAnimation;

  using RotationAnimation = mtt::KeypointsAnimatedValue<RotationKeypoint>;
  RotationAnimation _rotationAnimation;

  using ScaleAnimation = mtt::KeypointsAnimatedValue<ScaleKeypoint>;
  ScaleAnimation _scaleAnimation;

  TimeType _startTime;
  TimeType _finishTime;

  using SkeletonLink = mtt::ObjectLink< SkeletonObject,
                                        AnimationTrack,
                                        &AnimationTrack::_connectSkeleton,
                                        &AnimationTrack::_disconnectSkeleton>;
  std::optional<SkeletonLink> _skeletonLink;
};

inline bool AnimationTrack::enabled() const noexcept
{
  return _enabled;
}

inline void AnimationTrack::resetEnabled()
{
  setEnabled(true);
}

inline size_t AnimationTrack::positionKeypointNumber() const noexcept
{
  return _positionAnimation.keypointsNumber();
}

inline const AnimationTrack::PositionKeypoint&
                  AnimationTrack::positionKeypoint(size_t index) const noexcept
{
  return _positionAnimation.keypoint(index);
}

inline void AnimationTrack::addPositionKeypoint(
                                    std::unique_ptr<PositionKeypoint> keypoint)
{
  _positionAnimation.addKeypoint(std::move(keypoint));
  _updateTiming();
}

inline std::unique_ptr<AnimationTrack::PositionKeypoint>
                          AnimationTrack::removePositionKeypoint(
                                    const PositionKeypoint& keypoint) noexcept
{
  std::unique_ptr<AnimationTrack::PositionKeypoint> removedKeypoint =
                                    _positionAnimation.removeKeypoint(keypoint);
  _updateTiming();
  return removedKeypoint;
}

inline size_t AnimationTrack::rotationKeypointNumber() const noexcept
{
  return _rotationAnimation.keypointsNumber();
}

inline const AnimationTrack::RotationKeypoint&
                  AnimationTrack::rotationKeypoint(size_t index) const noexcept
{
  return _rotationAnimation.keypoint(index);
}

inline void AnimationTrack::addRotationKeypoint(
                                    std::unique_ptr<RotationKeypoint> keypoint)
{
  _rotationAnimation.addKeypoint(std::move(keypoint));
  _updateTiming();
}

inline std::unique_ptr<AnimationTrack::RotationKeypoint>
                          AnimationTrack::removeRotationKeypoint(
                                      const RotationKeypoint& keypoint) noexcept
{
  std::unique_ptr<AnimationTrack::RotationKeypoint> removedKeypoint =
                                    _rotationAnimation.removeKeypoint(keypoint);
  _updateTiming();
  return removedKeypoint;
}

inline size_t AnimationTrack::scaleKeypointNumber() const noexcept
{
  return _scaleAnimation.keypointsNumber();
}

inline const AnimationTrack::PositionKeypoint&
                      AnimationTrack::scaleKeypoint(size_t index) const noexcept
{
  return _scaleAnimation.keypoint(index);
}

inline void AnimationTrack::addScaleKeypoint(
                        std::unique_ptr<AnimationTrack::ScaleKeypoint> keypoint)
{
  _scaleAnimation.addKeypoint(std::move(keypoint));
  _updateTiming();
}

inline std::unique_ptr<AnimationTrack::ScaleKeypoint>
    AnimationTrack::removeScaleKeypoint(const ScaleKeypoint& keypoint) noexcept
{
  std::unique_ptr<AnimationTrack::ScaleKeypoint> removedKeypoint =
                                    _scaleAnimation.removeKeypoint(keypoint);
  _updateTiming();
  return removedKeypoint;
}

inline AnimationTrack::TimeType AnimationTrack::startTime() const noexcept
{
  return _startTime;
}

inline AnimationTrack::TimeType AnimationTrack::finishTime() const noexcept
{
  return _finishTime;
}

inline AnimationTrack::TimeType AnimationTrack::duration() const noexcept
{
  return finishTime() - startTime();
}

inline const mtt::ObjectRef<SkeletonObject>&
                                  AnimationTrack::skeletonRef() const noexcept
{
  return *_skeletonLink;
}

inline SkeletonObject* AnimationTrack::skeleton() const noexcept
{
  return _skeletonLink->get();
}
