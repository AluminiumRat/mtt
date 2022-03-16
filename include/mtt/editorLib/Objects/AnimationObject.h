#pragma once

#include <memory>

#include <QtCore/QTimer>

#include <mtt/application/EditCommands/AbstractEditCommand.h>
#include <mtt/application/Scene/SpecialGroup.h>
#include <mtt/application/TimeT.h>
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
    AnimationObject(const QString& name,
                    bool canBeRenamed,
                    const UID& id = UID());
    AnimationObject(const AnimationObject&) = delete;
    AnimationObject& operator = (const AnimationObject&) = delete;
    virtual ~AnimationObject() noexcept = default;

    inline const Range<TimeT>& timeRange() const noexcept;
    inline TimeT startTime() const noexcept;
    inline TimeT finishTime() const noexcept;
    inline TimeT duration() const noexcept;

    virtual void update(TimeT time);

    /// Makes a command to restore animated objects after the animation has
    /// played. The command has no undo functional.
    std::unique_ptr<AbstractEditCommand> makeRestoreCommand() const;

  signals:
    void childAdded(AnimationTrack& object);
    void childRemoved(AnimationTrack& object);
    void timeRangeChanged(const Range<TimeT>& newRange);

  protected:
    void updateTiming() noexcept;
    virtual Range<TimeT> calculateTiming() const noexcept;

    virtual void onSubobjectAdded(Object& object) noexcept override;
    virtual void onSubobjectRemoved(Object& object) noexcept override;

  private:
    Range<TimeT> _timeRange;
  };

  inline const Range<TimeT>& AnimationObject::timeRange() const noexcept
  {
    return _timeRange;
  }

  inline TimeT AnimationObject::startTime() const noexcept
  {
    return _timeRange.min();
  }

  inline TimeT AnimationObject::finishTime() const noexcept
  {
    return _timeRange.max();
  }

  inline TimeT AnimationObject::duration() const noexcept
  {
    return finishTime() - startTime();
  }
}