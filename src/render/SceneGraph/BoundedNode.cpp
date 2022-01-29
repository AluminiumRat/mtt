#include <algorithm>

#include <mtt/render/SceneGraph/BoundedNode.h>
#include <mtt/render/SceneGraph/BoundObserver.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

BoundedNode::~BoundedNode() noexcept
{
  for (BoundObserver* observer : _boundObservers)
  {
    observer->onNodeRemoved(*this);
  }
}

void BoundedNode::addBoundObserver(BoundObserver& observer)
{
  if(std::find( _boundObservers.begin(),
                _boundObservers.end(),
                &observer) != _boundObservers.end()) Abort("BoundedNode::addBoundObserver: observer is already registered.");
  _boundObservers.push_back(&observer);
}

void BoundedNode::removeBoundObserver(BoundObserver& observer) noexcept
{
  BoundObservers::iterator iObserver = std::find( _boundObservers.begin(),
                                                  _boundObservers.end(),
                                                  &observer);
  if(iObserver == _boundObservers.end()) return;
  _boundObservers.erase(iObserver);
}

void BoundedNode::setTransformMatrix(const glm::mat4& newValue) noexcept
{
  AbstractNode::setTransformMatrix(newValue);
  updateTransformedBoundSphere();
}

void BoundedNode::setLocalBoundSphere(const Sphere& newValue) noexcept
{
  _localBoundSphere = newValue;
  updateTransformedBoundSphere();
}

void BoundedNode::updateTransformedBoundSphere() noexcept
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
