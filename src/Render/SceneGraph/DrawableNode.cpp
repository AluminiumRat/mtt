#include <algorithm>

#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/Render/SceneGraph/BoundObserver.h>
#include <mtt/Render/SceneGraph/DrawableNode.h>

using namespace mtt;

DrawableNode::DrawableNode() noexcept:
  _normalTransformMatrix(1)
{
}

DrawableNode::~DrawableNode() noexcept
{
  for (BoundObserver* observer : _boundObservers)
  {
    observer->onNodeRemoved(*this);
  }
}

void DrawableNode::addBoundObserver(BoundObserver& observer)
{
  if(std::find( _boundObservers.begin(),
                _boundObservers.end(),
                &observer) != _boundObservers.end()) Abort("DrawableNode::addBoundObserver: observer is already registered.");
  _boundObservers.push_back(&observer);
}

void DrawableNode::removeBoundObserver(BoundObserver& observer) noexcept
{
  BoundObservers::iterator iObserver = std::find( _boundObservers.begin(),
                                                  _boundObservers.end(),
                                                  &observer);
  if(iObserver == _boundObservers.end()) return;
  _boundObservers.erase(iObserver);
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
  for (BoundObserver* observer : _boundObservers)
  {
    observer->onNodeBoundChanged(*this, oldValue);
  }
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
