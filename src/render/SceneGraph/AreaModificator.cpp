#include <atomic>

#include <mtt/render/SceneGraph/AreaModificator.h>

using namespace mtt;

AreaModificator::AreaModificator(size_t type) :
  _type(type)
{
}

size_t AreaModificator::allocateType() noexcept
{
  static std::atomic<size_t> typeCounter = 0;
  return typeCounter++;
}

void AreaModificator::buildPrepareActions(DrawPlanBuildInfo& buildInfo)
{
}
