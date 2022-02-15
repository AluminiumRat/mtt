#pragma once

#include <memory>

#include <QtCore/QTimer>

#include <mtt/application/EditCommands/AbstractEditCommand.h>
#include <mtt/application/Scene/SpecialGroup.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>

namespace mtt
{
  class AnimationObject : public SpecialGroup<Object, AnimationTrack>
  {
    Q_OBJECT

    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension)

  public:
    using TimeType = AnimationTrack::TimeType;

  public:
    AnimationObject(const QString& name,
                    bool canBeRenamed,
                    const UID& id = UID());
    AnimationObject(const AnimationObject&) = delete;
    AnimationObject& operator = (const AnimationObject&) = delete;
    virtual ~AnimationObject() noexcept = default;

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

    void startTimeChanged(TimeType newStartTime);
    void finishTimeChanged(TimeType newFinishTime);
    void durationChanged(TimeType newDuration);
    void timingChanged();

  protected:
    virtual void onSubobjectAdded(Object& object) noexcept override;
    virtual void onSubobjectRemoved(Object& object) noexcept override;

  private:
    void _updateTiming() noexcept;

  private:
    TimeType _startTime;
    TimeType _finishTime;
  };

  inline AnimationObject::TimeType AnimationObject::startTime() const noexcept
  {
    return _startTime;
  }

  inline AnimationObject::TimeType AnimationObject::finishTime() const noexcept
  {
    return _finishTime;
  }

  inline AnimationObject::TimeType AnimationObject::duration() const noexcept
  {
    return finishTime() - startTime();
  }
}