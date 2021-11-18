#include <mtt/DrawImpl/Lighting/AbstractLightApplicator.h>

using namespace mtt;

AbstractLightApplicator::AbstractLightApplicator() :
  _distance(100),
  _baseIlluminance(1, 1, 1)
{
}
