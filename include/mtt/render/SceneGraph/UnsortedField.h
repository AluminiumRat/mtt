#pragma once

#include <mtt/render/SceneGraph/AbstractField.h>
#include <mtt/render/SceneGraph/FieldArea.h>

namespace mtt
{
  class UnsortedField : public AbstractField
  {
  public:
    UnsortedField() = default;
    UnsortedField(const UnsortedField&) = delete;
    UnsortedField& operator = (const UnsortedField&) = delete;
    virtual ~UnsortedField() = default;

    virtual void addDrawable(DrawableNode& node) override;
    virtual void removeDrawable(DrawableNode& node) noexcept override;

  protected:
    virtual void enumerateAreas(Visitor& visitor,
                                const ViewFrustum& frustum) const override;
  private:
    FieldArea _area;
  };
}