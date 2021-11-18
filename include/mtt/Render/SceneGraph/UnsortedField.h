#pragma once

#include <mtt/Render/SceneGraph/VisitedField.h>

namespace mtt
{
  class UnsortedField : public VisitedField
  {
  public:
    UnsortedField() = default;
    UnsortedField(const UnsortedField&) = delete;
    UnsortedField& operator = (const UnsortedField&) = delete;
    virtual ~UnsortedField() = default;

    virtual void addNode(DrawableNode& node) override;
    virtual void removeNode(DrawableNode& node) noexcept override;

  protected:
    virtual void onNodeBoundChanged(DrawableNode& node,
                                    const Sphere& oldBound) noexcept override;

    virtual void enumerateAreas(Visitor& visitor,
                                const ViewFrustum& frustum) const override;
  private:
    Area _area;
  };
}