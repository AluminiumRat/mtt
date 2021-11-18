#pragma once

#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/Render/SceneGraph/VisitedField.h>

namespace mtt
{
  class DrawVisitor : public VisitedField::Visitor
  {
  public:
    explicit DrawVisitor(DrawPlanBuildInfo& buildInfo);
    DrawVisitor(const DrawVisitor&) = delete;
    DrawVisitor& operator = (const DrawVisitor&) = delete;
    virtual ~DrawVisitor() = default;

    virtual void startPass() override;
    virtual void visit(const VisitedField::Area& area) override;
    virtual void finishPass() override;

  private:
    DrawPlanBuildInfo& _buildInfo;
    ViewFrustum _localFrustum;
  };
}