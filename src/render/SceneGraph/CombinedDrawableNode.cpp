#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/CombinedDrawableNode.h>

using namespace mtt;

CombinedDrawableNode::~CombinedDrawableNode() noexcept
{
  for ( size_t childIndex = 0;
        childIndex < _drawables.childsNumber();
        childIndex++)
  {
    DrawableNode& child = static_cast<DrawableNode&>(
                                                  _drawables.child(childIndex));
    child.removeBoundObserver(*this);
  }
}

void CombinedDrawableNode::addNode(DrawableNode& node)
{
  _drawables.addChild(node);
  try
  {
    node.addBoundObserver(*this);
  }
  catch(...)
  {
    _drawables.removeChild(node);
    throw;
  }
  _updateBound();
}

void CombinedDrawableNode::removeNode(DrawableNode& node) noexcept
{
  _drawables.removeChild(node);
  node.removeBoundObserver(*this);
  _updateBound();
}

void CombinedDrawableNode::registerAreaModificators(AreaModificatorSet& set)
{
  _drawables.registerAreaModificators(set);
}

void CombinedDrawableNode::unregisterAreaModificators(
                                              AreaModificatorSet& set) noexcept
{
  _drawables.unregisterAreaModificators(set);
}

void CombinedDrawableNode::onNodeBoundChanged(BoundedNode& node,
                                              const Sphere& oldBound) noexcept
{
  _updateBound();
}

void CombinedDrawableNode::onNodeRemoved(BoundedNode& node) noexcept
{
  removeNode(static_cast<DrawableNode&>(node));
}

void CombinedDrawableNode::_updateBound() noexcept
{
  Sphere bound;
  for ( size_t childIndex = 0;
        childIndex < _drawables.childsNumber();
        childIndex++)
  {
    DrawableNode& child = static_cast<DrawableNode&>(
                                                  _drawables.child(childIndex));
    bound.extend(child.transformedBoundSphere());
  }

  updateBoundSphere(bound);
  setLocalBoundSphere(bound);
}

void CombinedDrawableNode::updateBoundSphere(Sphere& boundSphere) noexcept
{
}

void CombinedDrawableNode::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  _drawables.addToDrawPlan(buildInfo);
}
