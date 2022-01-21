#include <mtt/DLPipeline/Lighting/AbstractLightApplicator.h>

using namespace mtt;
using namespace mtt::DLPipeline;

AbstractLightApplicator::AbstractLightApplicator() :
  _distance(100),
  _baseIlluminance(1, 1, 1)
{
}
