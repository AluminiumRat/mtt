#pragma once

#include <vector>

#include <glm/mat4x4.hpp>

namespace mtt
{
  class DrawableNode;
  struct Sphere;
  class ViewFrustum;

  class AbstractField
  {
  public:
    AbstractField() noexcept = default;
    AbstractField(const AbstractField&) = delete;
    AbstractField& operator = (const AbstractField&) = delete;
    virtual ~AbstractField() noexcept = default;

    virtual void addNode(DrawableNode& node);
    virtual void removeNode(DrawableNode& node) noexcept;

  protected:
    friend class DrawableNode;
    virtual void onNodeBoundChanged(DrawableNode& node,
                                    const Sphere& oldBound) noexcept = 0;
  };
}