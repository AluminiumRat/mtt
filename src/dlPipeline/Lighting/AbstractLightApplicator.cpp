#include <mtt/dlPipeline/Lighting/AbstractLightApplicator.h>

using namespace mtt;
using namespace mtt::dlPipeline;

AbstractLightApplicator::AbstractLightApplicator() :
  _distance(100),
  _baseIlluminance(1, 1, 1)
{
}
