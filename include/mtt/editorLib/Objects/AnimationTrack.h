#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <memory>

#include <mtt/animation/ValueKeypoint.h>
#include <mtt/animation/KeypointsAnimatedTransform.h>
#include <mtt/application/EditCommands/AbstractEditCommand.h>
#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/application/Application.h>
#include <mtt/editorLib/Objects/CEVisitorExtension.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>

namespace mtt
{
  class AnimationTrack :  public Object,
                          public KeypointsAnimatedTransform<Application::TimeType>
  {
    Q_OBJECT

    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitAnimationTrack,
                            visitConstAnimationTrack,
                            Object)

    Q_PROPERTY( bool enabled
                READ enabled
                WRITE setEnabled
                RESET resetEnabled
                NOTIFY enabledChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

  public:
    using TimeType = Application::TimeType;
    using PositionKeypoint = ValueKeypoint<glm::vec3, TimeType>;
    using RotationKeypoint = ValueKeypoint<glm::quat, TimeType>;
    using ScaleKeypoint = ValueKeypoint<glm::vec3, TimeType>;

  public:
    AnimationTrack( const QString& name,
                    bool canBeRenamed,
                    const UID& id = UID());
    AnimationTrack(const AnimationTrack&) = delete;
    AnimationTrack& operator = (const AnimationTrack&) = delete;
    virtual ~AnimationTrack() noexcept = default;

    inline bool enabled() const noexcept;
    void setEnabled(bool newValue);
    inline void resetEnabled();

    void update(TimeType time);

    inline ObjectRef<SkeletonObject>& skeletonRef() noexcept;
    inline const ObjectRef<SkeletonObject>& skeletonRef() const noexcept;

    /// Makes a command to restore animated object after the animation has
    /// played. The command has no undo functional.
    std::unique_ptr<AbstractEditCommand> makeRestoreCommand() const;

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
    bool _enabled;

    using SkeletonLink = ObjectLink<SkeletonObject,
                                    AnimationTrack,
                                    nullptr,
                                    nullptr,
                                    &AnimationTrack::skeletonRefChanged>;
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

  inline ObjectRef<SkeletonObject>& AnimationTrack::skeletonRef() noexcept
  {
    return _skeletonLink;
  }

  inline const ObjectRef<SkeletonObject>&
                                    AnimationTrack::skeletonRef() const noexcept
  {
    return _skeletonLink;
  }
}