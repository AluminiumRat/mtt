#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/Render/SceneGraph/AbstractField.h>
#include <mtt/Render/SceneGraph/DrawableNode.h>

using namespace mtt;

DrawableNode::DrawableNode() noexcept:
  _field(nullptr),
  _normalTransformMatrix(1)
{
}

void DrawableNode::setTransformMatrix(const glm::mat4& newValue) noexcept
{
  AbstractNode::setTransformMatrix(newValue);
  _normalTransformMatrix = glm::transpose(glm::inverse(newValue));
  updateTransformedBoundSphere();
}

void DrawableNode::setLocalBoundSphere(const Sphere& newValue) noexcept
{
  _localBoundSphere = newValue;
  updateTransformedBoundSphere();
}

void DrawableNode::updateTransformedBoundSphere() noexcept
{
  Sphere oldValue = _transformedBoundSphere;
  _transformedBoundSphere = _localBoundSphere;
  if(_transformedBoundSphere.valid())
  {
    _transformedBoundSphere.translate(transformMatrix());
  }
  if(_field != nullptr) _field->onNodeBoundChanged(*this, oldValue);
}

void DrawableNode::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  DrawMatrices oldMatrices = buildInfo.drawMatrices;

  buildInfo.drawMatrices.localToViewMatrix =
                  buildInfo.drawMatrices.localToViewMatrix * transformMatrix();

  glm::mat3 oldNormalsMatrix = buildInfo.drawMatrices.normalsLocalToViewMatrix;
  buildInfo.drawMatrices.normalsLocalToViewMatrix =
                                    oldNormalsMatrix * _normalTransformMatrix;

  Drawable::addToDrawPlan(buildInfo);

  buildInfo.drawMatrices = oldMatrices;
}
