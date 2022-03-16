#pragma once

#include <memory>

#include <QtCore/QTimer>

#include <mtt/application/EditCommands/AbstractEditCommand.h>
#include <mtt/application/Scene/SpecialGroup.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/utilities/Range.h>

namespace mtt
{
  class AnimationObject : public SpecialGroup<Object, AnimationTrack>
  {
    Q_OBJECT

    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitAnimationObject,
                            visitConstAnimationObject,
                            Object)

  public:
    using TimeType = AnimationTrack::TimeType;

  public:
    AnimationObject(const QString& name,
                    bool canBeRenamed,
                    const UID& id = UID());
    AnimationObject(const AnimationObject&) = delete;
    AnimationObject& operator = (const AnimationObject&) = delete;
    virtual ~AnimationObject() noexcept = default;

    inline Range<TimeType> timeRange() const noexcept;
    inline TimeType startTime() const noexcept;
    inline TimeType finishTime() const noexcept;
    inline TimeType duration() const noexcept;

    void update(TimeType time);

    /// Makes a command to restore animated objects after the animation has
    /// played. The command has no undo functional.
    std::unique_ptr<AbstractEditCommand> makeRestoreCommand() const;

  signals:
    void childAdded(AnimationTrack& object);
    void childRemoved(AnimationTrack& object);

    void timeRangeChanged(Range<TimeType> newRange);

  protected:
    virtual void onSubobjectAdded(Object& object) noexcept override;
    virtual void onSubobjectRemoved(Object& object) noexcept override;

  private:
    void _updateTiming() noexcept;

  private:
    Range<TimeType> _timeRange;
  };

  inline Range<AnimationObject::TimeType>
                                    AnimationObject::timeRange() const noexcept
  {
    return _timeRange;
  }

  inline AnimationObject::TimeType AnimationObject::startTime() const noexcept
  {
    return _timeRange.min();
  }

  inline AnimationObject::TimeType AnimationObject::finishTime() const noexcept
  {
    return _timeRange.max();
  }

  inline AnimationObject::TimeType AnimationObject::duration() const noexcept
  {
    return finishTime() - startTime();
  }
}