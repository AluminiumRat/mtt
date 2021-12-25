#pragma once

#include <memory>
#include <vector>

#include <mtt/Application/DrawModel/DrawModelAnimationTrack.h>
#include <mtt/Application/DrawModel/TransformTable.h>
#include <mtt/Application/Application.h>

namespace mtt
{
  class DrawModelAnimation
  {
  public:
    DrawModelAnimation();
    DrawModelAnimation(const DrawModelAnimation&) = delete;
    DrawModelAnimation& operator = (const DrawModelAnimation&) = delete;
    virtual ~DrawModelAnimation() noexcept = default;

    inline Application::TimeType startTime() const noexcept;
    inline Application::TimeType finishTime() const noexcept;
    inline Application::TimeType duration() const noexcept;

    inline size_t tracksNumber() const noexcept;
    inline DrawModelAnimationTrack& track(size_t trackIndex) noexcept;
    inline const DrawModelAnimationTrack& track(
                                              size_t trackIndex) const noexcept;
    void addTrack(std::unique_ptr<DrawModelAnimationTrack> track);
    /// Returns removed track
    std::unique_ptr<DrawModelAnimationTrack> removeTrack(
                                      DrawModelAnimationTrack& track) noexcept;

    inline void updateTransform(TransformTable& target,
                                Application::TimeType time) const;

  private:
    friend class DrawModelAnimationTrack;
    void updateTiming() noexcept;

  private:
    Application::TimeType _startTime;
    Application::TimeType _finishTime;

    using Tracks = std::vector<std::unique_ptr<DrawModelAnimationTrack>>;
    Tracks _tracks;
  };

  inline Application::TimeType DrawModelAnimation::startTime() const noexcept
  {
    return _startTime;
  }

  inline Application::TimeType DrawModelAnimation::finishTime() const noexcept
  {
    return _finishTime;
  }

  inline Application::TimeType DrawModelAnimation::duration() const noexcept
  {
    return _finishTime - _startTime;
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
                                              Application::TimeType time) const
  {
    for ( size_t trackIndex = 0;
          trackIndex != _tracks.size();
          trackIndex++)
    {
      _tracks[trackIndex]->updateTransform(target, time);
    }
  }
}