#pragma once

#include <mtt/animation/KeypointsAnimatedTransform.h>
#include <mtt/application/DrawModel/TransformTable.h>
#include <mtt/application/TimeT.h>

namespace mtt
{
  class DrawModelAnimation;

  class DrawModelAnimationTrack : public KeypointsAnimatedTransform<TimeT>
  {
  public:

  public:
    DrawModelAnimationTrack();
    DrawModelAnimationTrack(const DrawModelAnimationTrack&) = delete;
    DrawModelAnimationTrack& operator = (
                                      const DrawModelAnimationTrack&) = delete;
    virtual ~DrawModelAnimationTrack() noexcept = default;

    inline TransformTable::Index boneIndex() const noexcept;
    inline void setBoneIndex(TransformTable::Index newValue) noexcept;

    inline void updateTransform(TransformTable& target, TimeT time) const;

  protected:
    virtual void onTimeRangeChanged() noexcept override;

  private:
    friend class DrawModelAnimation;
    inline void setAnimationPtr(DrawModelAnimation* animation) noexcept;

  private:
    DrawModelAnimation* _animation;
    TransformTable::Index _boneIndex;
  };

  inline TransformTable::Index
                            DrawModelAnimationTrack::boneIndex() const noexcept
  {
    return _boneIndex;
  }

  inline void DrawModelAnimationTrack::setBoneIndex(
                                        TransformTable::Index newValue) noexcept
  {
    _boneIndex = newValue;
  }

  inline void DrawModelAnimationTrack::updateTransform( TransformTable& target,
                                                        TimeT time) const
  {
    if(!timeRange().contains(time)) return;
    if(_boneIndex == TransformTable::notIndex) return;
    target.setTransform(_boneIndex, value(time));
  }

  inline void DrawModelAnimationTrack::setAnimationPtr(
                                        DrawModelAnimation* animation) noexcept
  {
    _animation = animation;
  }
}