#include <mtt/clPipeline/Lighting/LightAreaModificator.h>

using namespace mtt;
using namespace mtt::clPipeline;

const size_t LightAreaModificator::typeIndex = AreaModificator::allocateType();

LightAreaModificator::LightAreaModificator() :
  AreaModificator(typeIndex)
{
}
