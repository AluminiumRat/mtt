#pragma once

#include <vector>

namespace mtt
{
  class DrawableNode;
  class FieldArea;
  class ViewFrustum;

  class AbstractField
  {
  public:
    class Visitor
    {
    public:
      virtual void startPass() = 0;
      virtual void visit(const FieldArea&) = 0;
      virtual void finishPass() = 0;
    };

  public:
    AbstractField() noexcept = default;
    AbstractField(const AbstractField&) = delete;
    AbstractField& operator = (const AbstractField&) = delete;
    virtual ~AbstractField() noexcept = default;

    virtual void addDrawable(DrawableNode& node) = 0;
    virtual void removeDrawable(DrawableNode& node) noexcept = 0;

    inline void pass(Visitor& visitor, const ViewFrustum& frustum) const;

  protected:
    virtual void enumerateAreas(Visitor& visitor,
                                const ViewFrustum& frustum) const = 0;
  };

  inline void AbstractField::pass(Visitor& visitor,
                                  const ViewFrustum& frustum) const
  {
    visitor.startPass();
    enumerateAreas(visitor, frustum);
    visitor.finishPass();
  }
};