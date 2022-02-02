#include <mtt/render/SceneGraph/DrawableNode.h>
#include <mtt/render/SceneGraph/DrawVisitor.h>
#include <mtt/render/SceneGraph/FieldArea.h>

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

void DrawVisitor::visit(const FieldArea& area)
{
  if(area.drawables().empty()) return;

  const AreaModificatorSet* oldModificators = _buildInfo.areaModificators;
  _buildInfo.areaModificators = area.modificators();

  if(area.modificators() != nullptr)
  {
    for (AreaModificator* modificator : area.modificators()->modificators())
    {
      modificator->addToDrawPlan(_buildInfo);
    }
  }

  for(DrawableNode* node : area.drawables())
  {
    if(_localFrustum.intersect(node->transformedBoundSphere()))
    {
      node->addToDrawPlan(_buildInfo);
    }
  }

  _buildInfo.areaModificators = oldModificators;
}
