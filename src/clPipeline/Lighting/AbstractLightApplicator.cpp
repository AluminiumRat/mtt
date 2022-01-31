#include <mtt/clPipeline/Lighting/AbstractLightApplicator.h>

using namespace mtt;
using namespace mtt::clPipeline;

AbstractLightApplicator::AbstractLightApplicator() :
  _distance(100),
  _baseIlluminance(1, 1, 1)
{
}
