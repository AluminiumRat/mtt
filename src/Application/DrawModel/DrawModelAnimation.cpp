#include <mtt/Application/DrawModel/DrawModelAnimation.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

DrawModelAnimation::DrawModelAnimation() :
  _startTime(0),
  _finishTime(0)
{
}

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
  if (_tracks.empty())
  {
    _startTime = Application::TimeType(0);
    _finishTime = Application::TimeType(0);
  }

  _startTime = _tracks[0]->startTime();
  _finishTime = _tracks[0]->finishTime();

  for ( size_t trackIndex = 1;
        trackIndex < _tracks.size();
        trackIndex++)
  {
    _startTime = std::min(_startTime, _tracks[trackIndex]->startTime());
    _finishTime = std::min(_startTime, _tracks[trackIndex]->startTime());
  }
}
