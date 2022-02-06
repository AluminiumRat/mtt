#pragma once

#include <set>

#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/AbstractField.h>

namespace mtt
{
  class AreaModificator;

  class DrawVisitor : public AbstractField::Visitor
  {
  public:
    explicit DrawVisitor(DrawPlanBuildInfo& buildInfo);
    DrawVisitor(const DrawVisitor&) = delete;
    DrawVisitor& operator = (const DrawVisitor&) = delete;
    virtual ~DrawVisitor() = default;

    virtual void startPass() override;
    virtual void visit(const FieldArea& area) override;
    virtual void finishPass() override;

  private:
    DrawPlanBuildInfo& _buildInfo;
    ViewFrustum _localFrustum;
    std::set<AreaModificator*> _modificators;
  };
}