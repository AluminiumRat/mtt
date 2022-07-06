#pragma once

#include <set>
#include <vector>

#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/AbstractField.h>

namespace mtt
{
  class AreaModificator;

  class DrawVisitor : public AbstractField::Visitor
  {
  public:
    explicit DrawVisitor(DrawPlanBuildInfo& buildInfo);
    /// View frustum in local coordinates of visited field
    DrawVisitor(const std::vector<DrawPlanBuildInfo*>& buildInfos,
                const ViewFrustum& viewFrustum);
    DrawVisitor(const DrawVisitor&) = delete;
    DrawVisitor& operator = (const DrawVisitor&) = delete;
    virtual ~DrawVisitor() = default;

    virtual void startPass() override;
    virtual void visit(const FieldArea& area) override;
    virtual void finishPass() override;

  private:
    std::vector<DrawPlanBuildInfo*> _buildInfos;
    ViewFrustum _viewFrustum;
    std::set<AreaModificator*> _modificators;
  };
}