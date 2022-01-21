#pragma once

#include <mtt/render/SceneGraph/AbstractField.h>

namespace mtt
{
  class UnsortedField : public AbstractField
  {
  public:
    UnsortedField() = default;
    UnsortedField(const UnsortedField&) = delete;
    UnsortedField& operator = (const UnsortedField&) = delete;
    virtual ~UnsortedField() = default;

    virtual void addNode(DrawableNode& node) override;
    virtual void removeNode(DrawableNode& node) noexcept override;

  protected:
    virtual void enumerateAreas(Visitor& visitor,
                                const ViewFrustum& frustum) const override;
  private:
    Area _area;
  };
}