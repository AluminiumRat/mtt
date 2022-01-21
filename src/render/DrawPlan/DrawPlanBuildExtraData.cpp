#include <atomic>

#include <mtt/render/DrawPlan/DrawPlanBuildExtraData.h>

using namespace mtt;

const size_t DrawPlanBuildExtraData::selectedFlagIndex =
                                        DrawPlanBuildExtraData::allocateIndex();
const size_t DrawPlanBuildExtraData::objectUIDIndex =
                                        DrawPlanBuildExtraData::allocateIndex();
const size_t DrawPlanBuildExtraData::boneMatricesIndex =
                                        DrawPlanBuildExtraData::allocateIndex();

size_t DrawPlanBuildExtraData::allocateIndex() noexcept
{
  static std::atomic<size_t> indicesCounter(0);
  return indicesCounter++;
}
