#include <mtt/application/DrawModel/DrawModelAnimation.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

void DrawModelAnimation::addTrack(
                                std::unique_ptr<DrawModelAnimationTrack> track)
{
  if(track == nullptr) Abort("DrawModelAnimation::addTrack: track is null");
  DrawModelAnimationTrack& trackRef = *track;
  _tracks.push_back(std::move(track));
  trackRef.setAnimationPtr(this);
  updateTiming();
}

std::unique_ptr<DrawModelAnimationTrack> DrawModelAnimation::removeTrack(
                                        DrawModelAnimationTrack& track) noexcept
{
  std::unique_ptr<DrawModelAnimationTrack> removedTrack;

  for(Tracks::iterator iTrack = _tracks.begin();
      iTrack != _tracks.end();
      iTrack++)
  {
    if (iTrack->get() == &track)
    {
      removedTrack = std::move(*iTrack);
      _tracks.erase(iTrack);
      break;
    }
  }

  if(removedTrack != nullptr)
  {
    track.setAnimationPtr(nullptr);
    updateTiming();
  }

  return removedTrack;
}

void DrawModelAnimation::updateTiming() noexcept
{
  Range<TimeT> newTimeRange;

  for ( size_t trackIndex = 0;
        trackIndex < _tracks.size();
        trackIndex++)
  {
    newTimeRange.extend(_tracks[trackIndex]->timeRange());
  }

  _timeRange = newTimeRange;
}
