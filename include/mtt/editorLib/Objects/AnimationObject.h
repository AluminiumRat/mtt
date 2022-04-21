#pragma once

#include <memory>

#include <QtCore/QTimer>

#include <mtt/application/Scene/SpecialGroup.h>
#include <mtt/application/TimeT.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>

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
    AnimationObject(const QString& name,
                    bool canBeRenamed,
                    const UID& id = UID());
    AnimationObject(const AnimationObject&) = delete;
    AnimationObject& operator = (const AnimationObject&) = delete;
    virtual ~AnimationObject() noexcept = default;

    inline const TimeRange& timeRange() const noexcept;

    virtual void update(TimeRange time);

    /// Makes a command to restore animated objects after the animation has
    /// played. The command has no undo functional.
    std::unique_ptr<AbstractEditCommand> makeRestoreCommand() const;

  signals:
    void childAdded(AnimationTrack& object);
    void childRemoved(AnimationTrack& object);
    void timeRangeChanged(const TimeRange& newRange);

  protected:
    void updateTiming() noexcept;
    virtual TimeRange calculateTiming() const noexcept;

    virtual void onSubobjectAdded(Object& object) noexcept override;
    virtual void onSubobjectRemoved(Object& object) noexcept override;

  private:
    TimeRange _timeRange;
  };

  inline const TimeRange& AnimationObject::timeRange() const noexcept
  {
    return _timeRange;
  }
}