#include <mtt/application/DrawModel/DrawModelAnimationTrack.h>
#include <mtt/application/DrawModel/DrawModelAnimation.h>

using namespace mtt;

DrawModelAnimationTrack::DrawModelAnimationTrack() :
  _animation(nullptr),
  _boneIndex(TransformTable::notIndex)
{
}

void DrawModelAnimationTrack::onTimingChanged() noexcept
{
  if(_animation != nullptr) _animation->updateTiming();
}
