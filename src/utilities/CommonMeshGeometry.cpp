#include <mtt/utilities/CommonMeshGeometry.h>

using namespace mtt;

Box CommonMeshGeometry::calculateBoundingBox() const noexcept
{
  Box bound;
  for(const glm::vec3& position : positions) bound.extend(position);
  return bound;
}
