#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <mtt/animation/ValueKeypoint.h>
#include <mtt/animation/KeypointsAnimatedTransform.h>

#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/editorLib/Objects/CEVisitorExtension.h>
#include <mtt/editorLib/Objects/ScalableObject.h>

namespace mtt
{
  class PositionAnimator :  public AnimationTrack,
                            public KeypointsAnimatedTransform<TimeT>
  {
    Q_OBJECT

    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitPositionAnimator,
                            visitConstPositionAnimator,
                            AnimationTrack)

  public:
    using PositionKeypoint = ValueKeypoint<glm::vec3, TimeT>;
    using RotationKeypoint = ValueKeypoint<glm::quat, TimeT>;
    using ScaleKeypoint = ValueKeypoint<glm::vec3, TimeT>;

  public:
    PositionAnimator( const QString& name,
                      bool canBeRenamed,
                      const UID& id = UID());
    PositionAnimator(const PositionAnimator&) = delete;
    PositionAnimator& operator = (const PositionAnimator&) = delete;
    virtual ~PositionAnimator() noexcept = default;

    inline ObjectRef<ScalableObject>& targetRef() noexcept;
    inline const ObjectRef<ScalableObject>& targetRef() const noexcept;

    virtual void update(TimeRange time) override;

    virtual std::unique_ptr<AbstractEditCommand>
                                              makeRestoreCommand() const override;

    inline const TimeRange& timeRange() const noexcept;

  signals:
    void targetRefChanged(ScalableObject* target);

  protected:
    virtual void onTimeRangeChanged() noexcept override;

  private:
    using TargetLink = ObjectLink<ScalableObject,
                                  PositionAnimator,
                                  nullptr,
                                  nullptr,
                                  &PositionAnimator::targetRefChanged>;
    TargetLink _targetLink;
  };

  inline ObjectRef<ScalableObject>& PositionAnimator::targetRef() noexcept
  {
    return _targetLink;
  }

  inline const ObjectRef<ScalableObject>&
                                    PositionAnimator::targetRef() const noexcept
  {
    return _targetLink;
  }

  inline const TimeRange& PositionAnimator::timeRange() const noexcept
  {
    return AnimationTrack::timeRange();
  }
}