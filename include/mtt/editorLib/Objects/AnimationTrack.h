#pragma once

#include <mtt/application/EditCommands/AbstractEditCommand.h>
#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/application/TimeT.h>
#include <mtt/editorLib/Objects/CEVisitorExtension.h>

namespace mtt
{
  class AnimationTrack :  public Object
  {
    Q_OBJECT

    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitAnimationTrack,
                            visitConstAnimationTrack,
                            Object)

    Q_PROPERTY( bool enabled
                READ enabled
                WRITE setEnabled
                NOTIFY enabledChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)

  public:
    AnimationTrack( const QString& name,
                    bool canBeRenamed,
                    const UID& id = UID());
    AnimationTrack(const AnimationTrack&) = delete;
    AnimationTrack& operator = (const AnimationTrack&) = delete;
    virtual ~AnimationTrack() noexcept = default;

    inline bool enabled() const noexcept;
    void setEnabled(bool newValue);

    inline const TimeRange& timeRange() const noexcept;

    virtual void update(TimeRange time) = 0;

    /// Makes a command to restore animated object after the animation has
    /// played. The command has no undo functional.
    /// Can be returns nullptr if track have no affect to objects model
    virtual std::unique_ptr<AbstractEditCommand> makeRestoreCommand() const = 0;

  signals:
    void enabledChanged(bool newValue);
    void timeRangeChanged(const TimeRange& newValue);

  protected:
    void setTimeRange(const TimeRange& newValue) noexcept;

  private:
    bool _enabled;
    TimeRange _timeRange;
  };

  inline bool AnimationTrack::enabled() const noexcept
  {
    return _enabled;
  }

  inline const TimeRange& AnimationTrack::timeRange() const noexcept
  {
    return _timeRange;
  }
}