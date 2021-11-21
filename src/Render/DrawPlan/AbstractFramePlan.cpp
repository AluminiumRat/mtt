#include <mtt/Render/DrawPlan/AbstractFramePlan.h>
#include <mtt/Render/SceneRenderer/AbstractFrame.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

AbstractFramePlan::AbstractFramePlan(AbstractFrame& theFrame) :
  _memoryPool(1024 * 1024),
  _frame(theFrame),
  _frameBuilder(theFrame.builder())
{
  _bins.reserve(64);
}

void AbstractFramePlan::registerBin(DrawBin& bin, StageIndex stage)
{
  if(stage.index() < 0) Abort("AbstractFramePlan::registerBin: stage index < 0");
  if(_bins.size() <= stage.index())
  {
    _bins.resize(size_t(stage.index() + 1), nullptr);
  }
  if(_bins[stage.index()] != nullptr) Abort("AbstractFramePlan::registerBin: draw bin for this stage is already registered.");
  _bins[stage.index()] = &bin;
}
