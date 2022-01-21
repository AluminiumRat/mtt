#pragma once

#include <memory>

#include <mtt/animation/KeypointsAnimatedTransform.h>
#include <mtt/application/EditCommands/AbstractEditCommand.h>
#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/application/Application.h>

#include <Objects/OEVisitorExtension.h>
#include <Objects/SkeletonObject.h>

class AnimationTrack :
              public mtt::Object,
              public mtt::KeypointsAnimatedTransform<mtt::Application::TimeType>
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
  using TimeType = mtt::Application::TimeType;
  using PositionKeypoint = mtt::ValueKeypoint<glm::vec3, TimeType>;
  using RotationKeypoint = mtt::ValueKeypoint<glm::quat, TimeType>;
  using ScaleKeypoint = mtt::ValueKeypoint<glm::vec3, TimeType>;

public:
  AnimationTrack( const QString& name,
                  bool canBeRenamed,
                  const mtt::UID& id = mtt::UID());
  AnimationTrack(const AnimationTrack&) = delete;
  AnimationTrack& operator = (const AnimationTrack&) = delete;
  virtual ~AnimationTrack() noexcept = default;

  inline bool enabled() const noexcept;
  void setEnabled(bool newValue);
  inline void resetEnabled();

  void update(TimeType time);

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

protected:
  virtual void onStartTimeChanged() noexcept override;
  virtual void onFinishTimeChanged() noexcept override;
  virtual void onTimingChanged() noexcept override;
  virtual void onDurationChanged() noexcept override;

private:
  void _connectSkeleton(SkeletonObject& skeleton);
  void _disconnectSkeleton(SkeletonObject& skeleton) noexcept;

private:
  bool _enabled;

  using SkeletonLink = mtt::ObjectLink< SkeletonObject,
                                        AnimationTrack,
                                        &AnimationTrack::_connectSkeleton,
                                        &AnimationTrack::_disconnectSkeleton>;
  SkeletonLink _skeletonLink;
};

inline bool AnimationTrack::enabled() const noexcept
{
  return _enabled;
}

inline void AnimationTrack::resetEnabled()
{
  setEnabled(true);
}

inline const mtt::ObjectRef<SkeletonObject>&
                                  AnimationTrack::skeletonRef() const noexcept
{
  return _skeletonLink;
}

inline SkeletonObject* AnimationTrack::skeleton() const noexcept
{
  return _skeletonLink.get();
}
