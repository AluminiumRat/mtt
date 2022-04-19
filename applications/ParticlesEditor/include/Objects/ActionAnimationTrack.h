#pragma once

#include <mtt/editorLib/Objects/AnimationTrack.h>

#include <Objects/PEVisitorExtension.h>

class ActionAnimationTrack : public mtt::AnimationTrack
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitActionAnimationTrack,
                          visitConstActionAnimationTrack,
                          mtt::AnimationTrack)

  Q_PROPERTY( mtt::TimeT startTime
              READ startTime
              WRITE setStartTime
              RESET resetStartTime
              NOTIFY startTimeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::TimeT duration
              READ duration
              WRITE setDuration
              RESET resetDuration
              NOTIFY durationChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  ActionAnimationTrack( const QString& name,
                        bool canBeRenamed,
                        const mtt::UID& id = mtt::UID());
  ActionAnimationTrack(const ActionAnimationTrack&) = delete;
  ActionAnimationTrack& operator = (const ActionAnimationTrack&) = delete;
  virtual ~ActionAnimationTrack() noexcept = default;

  inline mtt::TimeT startTime() const noexcept;
  void setStartTime(mtt::TimeT newValue) noexcept;
  inline void resetStartTime() noexcept;

  inline mtt::TimeT duration() const noexcept;
  void setDuration(mtt::TimeT newValue) noexcept;
  inline void resetDuration() noexcept;

  virtual void update(TimeRange time) override;
  virtual std::unique_ptr<mtt::AbstractEditCommand>
                                            makeRestoreCommand() const override;

signals:
  void startTimeChanged(mtt::TimeT newValue);
  void durationChanged(mtt::TimeT newValue);

protected:
  virtual void makeAction(float portion) = 0;

private:
  mtt::TimeT _startTime;
  mtt::TimeT _duration;
};

inline mtt::TimeT ActionAnimationTrack::startTime() const noexcept
{
  return _startTime;
}

inline void ActionAnimationTrack::resetStartTime() noexcept
{
  setStartTime(mtt::TimeT(0));
}

inline mtt::TimeT ActionAnimationTrack::duration() const noexcept
{
  return _duration;
}

inline void ActionAnimationTrack::resetDuration() noexcept
{
  setDuration(mtt::TimeT(0));
}
