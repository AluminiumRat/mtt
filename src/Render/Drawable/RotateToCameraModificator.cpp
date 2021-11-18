#include <glm/gtx/transform.hpp>

#include <mtt/Render/Drawable/RotateToCameraModificator.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/Render/SceneGraph/CameraNode.h>

using namespace mtt;

void RotateToCameraModificator::draw(DrawPlanBuildInfo& buildInfo,
                                        Modificator** next,
                                        size_t modifiactorsLeft,
                                        Drawable& drawable) const
{
  DrawMatrices oldMatrices = buildInfo.drawMatrices;

  float xScale = glm::length(buildInfo.drawMatrices.localToViewMatrix[0]);
  buildInfo.drawMatrices.localToViewMatrix[0] = glm::vec4(xScale, 0, 0, 0);
  
  float yScale = glm::length(buildInfo.drawMatrices.localToViewMatrix[1]);
  buildInfo.drawMatrices.localToViewMatrix[1] = glm::vec4(0, yScale, 0, 0);

  float zScale = glm::length(buildInfo.drawMatrices.localToViewMatrix[2]);
  buildInfo.drawMatrices.localToViewMatrix[2] = glm::vec4(0, 0, zScale, 0);

  drawNext(buildInfo, next, modifiactorsLeft, drawable);

  buildInfo.drawMatrices = oldMatrices;
}
