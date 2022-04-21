#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/DrawableNode.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

DrawableNode::DrawableNode() :
  _normalTransformMatrix(1)
{
}

void DrawableNode::setTransformMatrix(const glm::mat4& newValue) noexcept
{
  _normalTransformMatrix = glm::transpose(glm::inverse(newValue));
  BoundedNode::setTransformMatrix(newValue);
}

void DrawableNode::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  ValueRestorer<DrawMatrices> restoreMatrices(buildInfo.drawMatrices);

  buildInfo.drawMatrices.localToViewMatrix =
                  buildInfo.drawMatrices.localToViewMatrix * transformMatrix();

  glm::mat3 oldNormalsMatrix = buildInfo.drawMatrices.normalsLocalToViewMatrix;
  buildInfo.drawMatrices.normalsLocalToViewMatrix =
                                    oldNormalsMatrix * _normalTransformMatrix;

  Drawable::addToDrawPlan(buildInfo);
}
