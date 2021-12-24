#pragma once

#include <mtt/Animation/KeypointsAnimatedTransform.h>
#include <mtt/Application/DrawModel/DrawModelTransformTable.h>
#include <mtt/Application/Application.h>

namespace mtt
{
  class DrawModelAnimation;

  class DrawModelAnimationTrack :
                      public KeypointsAnimatedTransform<Application::TimeType>
  {
  public:

  public:
    DrawModelAnimationTrack();
    DrawModelAnimationTrack(const DrawModelAnimationTrack&) = delete;
    DrawModelAnimationTrack& operator = (
                                      const DrawModelAnimationTrack&) = delete;
    virtual ~DrawModelAnimationTrack() noexcept = default;

    inline DrawModelTransformTable::Index boneIndex() const noexcept;
    inline void setBoneIndex(DrawModelTransformTable::Index newValue) noexcept;

    inline void updateTransform(DrawModelTransformTable& target,
                                Application::TimeType time) const;

  protected:
    virtual void onTimingChanged() noexcept override;

  private:
    friend class DrawModelAnimation;
    inline void setAnimationPtr(DrawModelAnimation* animation) noexcept;

  private:
    DrawModelAnimation* _animation;
    DrawModelTransformTable::Index _boneIndex;
  };

  inline DrawModelTransformTable::Index
                      DrawModelAnimationTrack::boneIndex() const noexcept
  {
    return _boneIndex;
  }

  inline void DrawModelAnimationTrack::setBoneIndex(
                              DrawModelTransformTable::Index newValue) noexcept
  {
    _boneIndex = newValue;
  }

  inline void DrawModelAnimationTrack::updateTransform(
                                              DrawModelTransformTable& target,
                                              Application::TimeType time) const
  {
    if(_boneIndex == DrawModelTransformTable::notIndex) return;
    target.setTransform(_boneIndex, value(time));
  }

  inline void DrawModelAnimationTrack::setAnimationPtr(
                                        DrawModelAnimation* animation) noexcept
  {
    _animation = animation;
  }
}