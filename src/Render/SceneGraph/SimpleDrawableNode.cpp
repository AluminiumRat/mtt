#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/Render/SceneGraph/SimpleDrawableNode.h>

using namespace mtt;

SimpleDrawableNode::SimpleDrawableNode() noexcept :
  _drawable(nullptr)
{
}

void SimpleDrawableNode::setDrawable( Drawable* newDrawable,
                                      const Sphere& localBoundSphere) noexcept
{
  _drawable = newDrawable;
  setLocalBoundSphere(localBoundSphere);
}

void SimpleDrawableNode::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  if(_drawable != nullptr) _drawable->addToDrawPlan(buildInfo);
}
