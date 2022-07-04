#pragma once

#include <mtt/editorLib/Objects/AnimationTrack.h>

#include <Objects/PEVisitorExtension.h>

class AnimationAction : public mtt::AnimationTrack
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitAnimationAction,
                          visitConstAnimationAction,
                          mtt::AnimationTrack)

  Q_PROPERTY( mtt::TimeT startTime
              READ startTime
              WRITE setStartTime
              NOTIFY startTimeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::TimeT duration
              READ duration
              WRITE setDuration
              NOTIFY durationChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  AnimationAction(const QString& name,
                  bool canBeRenamed,
                  const mtt::UID& id = mtt::UID());
  AnimationAction(const AnimationAction&) = delete;
  AnimationAction& operator = (const AnimationAction&) = delete;
  virtual ~AnimationAction() noexcept = default;

  inline mtt::TimeT startTime() const noexcept;
  void setStartTime(mtt::TimeT newValue) noexcept;

  inline mtt::TimeT duration() const noexcept;
  void setDuration(mtt::TimeT newValue) noexcept;

  virtual void update(mtt::TimeRange time) override;
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

inline mtt::TimeT AnimationAction::startTime() const noexcept
{
  return _startTime;
}

inline mtt::TimeT AnimationAction::duration() const noexcept
{
  return _duration;
}
