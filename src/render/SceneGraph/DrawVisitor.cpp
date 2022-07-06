#include <mtt/render/SceneGraph/DrawableNode.h>
#include <mtt/render/SceneGraph/DrawVisitor.h>
#include <mtt/render/SceneGraph/FieldArea.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

DrawVisitor::DrawVisitor(DrawPlanBuildInfo& buildInfo) :
  _buildInfos{&buildInfo},
  _viewFrustum(buildInfo.viewFrustum)
{
  _viewFrustum.fastTranslate(
                      glm::transpose(buildInfo.drawMatrices.localToViewMatrix));
}

DrawVisitor::DrawVisitor( const std::vector<DrawPlanBuildInfo*>& buildInfos,
                          const ViewFrustum& viewFrustum) :
  _buildInfos(buildInfos),
  _viewFrustum(viewFrustum)
{
}

void DrawVisitor::startPass()
{
  _modificators.clear();
}

void DrawVisitor::finishPass()
{
  for(DrawPlanBuildInfo* buildInfo : _buildInfos)
  {
    for (AreaModificator* modificator : _modificators)
    {
      modificator->addToDrawPlan(*buildInfo);
    }
  }
}

void DrawVisitor::visit(const FieldArea& area)
{
  if(area.drawables().empty()) return;

  if(area.modificators() != nullptr)
  {
    for (AreaModificator* modificator : area.modificators()->modificators())
    {
      _modificators.insert(modificator);
    }
  }

  for (DrawPlanBuildInfo* buildInfo : _buildInfos)
  {
    ScopedSetter<const AreaModificatorSet*> setAreaModificators(
                                                    buildInfo->areaModificators,
                                                    area.modificators());

    for(DrawableNode* node : area.drawables())
    {
      if(_viewFrustum.intersect(node->transformedBoundSphere()))
      {
        node->addToDrawPlan(*buildInfo);
      }
    }
  }
}
