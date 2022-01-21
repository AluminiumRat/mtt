#pragma once

#include <vector>

#include <glm/mat4x4.hpp>

namespace mtt
{
  class DrawableNode;
  struct Sphere;
  class ViewFrustum;

  class BoundObserver
  {
  public:
    BoundObserver() noexcept = default;
    BoundObserver(const BoundObserver&) = delete;
    BoundObserver& operator = (const BoundObserver&) = delete;
    virtual ~BoundObserver() noexcept = default;

  protected:
    friend class DrawableNode;
    virtual void onNodeBoundChanged(DrawableNode& node,
                                    const Sphere& oldBound) noexcept = 0;
    virtual void onNodeRemoved(DrawableNode& node) noexcept = 0;
  };
}