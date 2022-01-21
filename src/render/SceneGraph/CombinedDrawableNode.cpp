#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/CombinedDrawableNode.h>

using namespace mtt;

CombinedDrawableNode::~CombinedDrawableNode() noexcept
{
  for (DrawableNode* node : _drawables)
  {
    node->removeBoundObserver(*this);
  }
}

void CombinedDrawableNode::addNode(DrawableNode& node)
{
  _drawables.push_back(&node);
  try
  {
    node.addBoundObserver(*this);
  }
  catch(...)
  {
    _drawables.pop_back();
    throw;
  }
  _updateBound();
}

void CombinedDrawableNode::removeNode(DrawableNode& node) noexcept
{
  for(Drawables::iterator iDrawable = _drawables.begin();
      iDrawable != _drawables.end();
      iDrawable++)
  {
    if(*iDrawable == &node)
    {
      _drawables.erase(iDrawable);
      break;
    }
  }
  node.removeBoundObserver(*this);
  _updateBound();
}

void CombinedDrawableNode::onNodeBoundChanged(DrawableNode& node,
                                              const Sphere& oldBound) noexcept
{
  _updateBound();
}

void CombinedDrawableNode::onNodeRemoved(DrawableNode& node) noexcept
{
  removeNode(node);
}

void CombinedDrawableNode::_updateBound() noexcept
{
  Sphere bound;
  for(DrawableNode* drawable : _drawables)
  {
    bound.extend(drawable->transformedBoundSphere());
  }
  updateBoundSphere(bound);
  setLocalBoundSphere(bound);
}

void CombinedDrawableNode::updateBoundSphere(Sphere& boundSphere) noexcept
{
}

void CombinedDrawableNode::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  for (DrawableNode* drawable : _drawables)
  {
    drawable->addToDrawPlan(buildInfo);
  }
}
