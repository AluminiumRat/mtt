#include <atomic>

#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/AreaModificator.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

AreaModificator::AreaModificator(size_t type) :
  _type(type)
{
}

size_t AreaModificator::allocateType() noexcept
{
  static std::atomic<size_t> typeCounter = 0;
  return typeCounter++;
}

void AreaModificator::setTransformMatrix(const glm::mat4& newValue) noexcept
{
  _normalTransformMatrix = glm::transpose(glm::inverse(newValue));
  BoundedNode::setTransformMatrix(newValue);
}

void AreaModificator::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  if(_consumers.empty()) return;

  ValueRestorer<DrawMatrices> restoreMatrices(buildInfo.drawMatrices);

  buildInfo.drawMatrices.localToViewMatrix =
                  buildInfo.drawMatrices.localToViewMatrix * transformMatrix();

  glm::mat3 oldNormalsMatrix = buildInfo.drawMatrices.normalsLocalToViewMatrix;
  buildInfo.drawMatrices.normalsLocalToViewMatrix =
                                    oldNormalsMatrix * _normalTransformMatrix;

  buildPrepareActions(buildInfo);
}

void AreaModificator::buildPrepareActions(DrawPlanBuildInfo& buildInfo)
{
}
