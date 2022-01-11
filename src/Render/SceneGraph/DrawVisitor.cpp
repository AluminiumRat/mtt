#include <mtt/Render/SceneGraph/DrawableNode.h>
#include <mtt/Render/SceneGraph/DrawVisitor.h>

using namespace mtt;

DrawVisitor::DrawVisitor(DrawPlanBuildInfo& buildInfo) :
  _buildInfo(buildInfo),
  _localFrustum(_buildInfo.viewFrustum)
{
  _localFrustum.fastTranslate(
                    glm::transpose(_buildInfo.drawMatrices.localToViewMatrix));
}

void DrawVisitor::startPass()
{
}

void DrawVisitor::finishPass()
{
}

void DrawVisitor::visit(const AbstractField::Area& area)
{
  for(DrawableNode* node : area.nodes)
  {
    if(_localFrustum.intersect(node->transformedBoundSphere()))
    {
      node->addToDrawPlan(_buildInfo);
    }
  }
}
