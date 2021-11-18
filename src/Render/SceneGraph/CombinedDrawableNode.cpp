#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/Render/SceneGraph/CombinedDrawableNode.h>

using namespace mtt;

void CombinedDrawableNode::addNode(DrawableNode& node)
{
  _drawables.push_back(&node);
  try
  {
    AbstractField::addNode(node);
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
  AbstractField::removeNode(node);
  _updateBound();
}

void CombinedDrawableNode::onNodeBoundChanged(DrawableNode& node,
                                              const Sphere& oldBound) noexcept
{
  _updateBound();
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
