#include <glm/gtx/transform.hpp>

#include <mtt/render/Drawable/RotateToCameraModificator.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

void RotateToCameraModificator::draw( DrawPlanBuildInfo& buildInfo,
                                      DrawableModificator** next,
                                      size_t modifiactorsLeft,
                                      Drawable& drawable) const
{
  ValueRestorer<DrawMatrices> restoreMatrices(buildInfo.drawMatrices);

  float xScale = glm::length(buildInfo.drawMatrices.localToViewMatrix[0]);
  buildInfo.drawMatrices.localToViewMatrix[0] = glm::vec4(xScale, 0, 0, 0);
  
  float yScale = glm::length(buildInfo.drawMatrices.localToViewMatrix[1]);
  buildInfo.drawMatrices.localToViewMatrix[1] = glm::vec4(0, yScale, 0, 0);

  float zScale = glm::length(buildInfo.drawMatrices.localToViewMatrix[2]);
  buildInfo.drawMatrices.localToViewMatrix[2] = glm::vec4(0, 0, zScale, 0);

  drawNext(buildInfo, next, modifiactorsLeft, drawable);
}
