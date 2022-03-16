#pragma once

#include <memory>
#include <vector>

#include <mtt/application/DrawModel/DrawModelAnimationTrack.h>
#include <mtt/application/DrawModel/TransformTable.h>
#include <mtt/application/Application.h>
#include <mtt/utilities/Range.h>

namespace mtt
{
  class DrawModelAnimation
  {
  public:
    using TimeType = Application::TimeType;

  public:
    DrawModelAnimation() = default;
    DrawModelAnimation(const DrawModelAnimation&) = delete;
    DrawModelAnimation& operator = (const DrawModelAnimation&) = delete;
    virtual ~DrawModelAnimation() noexcept = default;

    inline Range<TimeType> timeRange() const noexcept;
    inline TimeType startTime() const noexcept;
    inline TimeType finishTime() const noexcept;
    inline TimeType duration() const noexcept;

    inline size_t tracksNumber() const noexcept;
    inline DrawModelAnimationTrack& track(size_t trackIndex) noexcept;
    inline const DrawModelAnimationTrack& track(
                                              size_t trackIndex) const noexcept;
    void addTrack(std::unique_ptr<DrawModelAnimationTrack> track);
    /// Returns removed track
    std::unique_ptr<DrawModelAnimationTrack> removeTrack(
                                      DrawModelAnimationTrack& track) noexcept;

    inline void updateTransform(TransformTable& target,
                                TimeType time) const;

  private:
    friend class DrawModelAnimationTrack;
    void updateTiming() noexcept;

  private:
    Range<TimeType> _timeRange;

    using Tracks = std::vector<std::unique_ptr<DrawModelAnimationTrack>>;
    Tracks _tracks;
  };

  inline Range<DrawModelAnimation::TimeType>
                                  DrawModelAnimation::timeRange() const noexcept
  {
    return _timeRange;
  }

  inline DrawModelAnimation::TimeType
                                  DrawModelAnimation::startTime() const noexcept
  {
    return _timeRange.min();
  }

  inline DrawModelAnimation::TimeType
                                DrawModelAnimation::finishTime() const noexcept
  {
    return _timeRange.max();
  }

  inline DrawModelAnimation::TimeType DrawModelAnimation::duration() const noexcept
  {
    return finishTime() - startTime();
  }

  inline size_t DrawModelAnimation::tracksNumber() const noexcept
  {
    return _tracks.size();
  }

  inline DrawModelAnimationTrack& DrawModelAnimation::track(
                                                    size_t trackIndex) noexcept
  {
    return *_tracks[trackIndex];
  }

  inline const DrawModelAnimationTrack& DrawModelAnimation::track(
                                              size_t trackIndex) const noexcept
  {
    return *_tracks[trackIndex];
  }

  inline void DrawModelAnimation::updateTransform(
                                              TransformTable& target,
                                              TimeType time) const
  {
    for ( size_t trackIndex = 0;
          trackIndex != _tracks.size();
          trackIndex++)
    {
      _tracks[trackIndex]->updateTransform(target, time);
    }
  }
}