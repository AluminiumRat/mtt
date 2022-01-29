#pragma once

#include <vector>

#include <glm/mat4x4.hpp>

namespace mtt
{
  class BoundedNode;
  struct Sphere;

  class BoundObserver
  {
  public:
    BoundObserver() noexcept = default;
    BoundObserver(const BoundObserver&) = delete;
    BoundObserver& operator = (const BoundObserver&) = delete;
    virtual ~BoundObserver() noexcept = default;

  protected:
    friend class BoundedNode;
    virtual void onNodeBoundChanged(BoundedNode& node,
                                    const Sphere& oldBound) noexcept = 0;
    virtual void onNodeRemoved(BoundedNode& node) noexcept = 0;
  };
}