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
#include <mtt/editorLib/Objects/ScalableObject.h>

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

    inline ObjectRef<ScalableObject>& targetRef() noexcept;
    inline const ObjectRef<ScalableObject>& targetRef() const noexcept;

    /// Makes a command to restore animated object after the animation has
    /// played. The command has no undo functional.
    std::unique_ptr<AbstractEditCommand> makeRestoreCommand() const;

  signals:
    void enabledChanged(bool newValue);
    void startTimeChanged(TimeType newStartTime);
    void finishTimeChanged(TimeType newFinishTime);
    void durationChanged(TimeType newDuration);
    void timingChanged();
    void targetRefChanged(ScalableObject* target);

  protected:
    virtual void onStartTimeChanged() noexcept override;
    virtual void onFinishTimeChanged() noexcept override;
    virtual void onTimingChanged() noexcept override;
    virtual void onDurationChanged() noexcept override;

  private:
    bool _enabled;

    using TargetLink = ObjectLink<ScalableObject,
                                  AnimationTrack,
                                  nullptr,
                                  nullptr,
                                  &AnimationTrack::targetRefChanged>;
    TargetLink _targetLink;
  };

  inline bool AnimationTrack::enabled() const noexcept
  {
    return _enabled;
  }

  inline void AnimationTrack::resetEnabled()
  {
    setEnabled(true);
  }

  inline ObjectRef<ScalableObject>& AnimationTrack::targetRef() noexcept
  {
    return _targetLink;
  }

  inline const ObjectRef<ScalableObject>&
                                      AnimationTrack::targetRef() const noexcept
  {
    return _targetLink;
  }
}