#pragma once

#include <vector>

#include <glm/mat4x4.hpp>

#include <mtt/render/SceneGraph/AbstractNode.h>
#include <mtt/utilities/Sphere.h>

namespace mtt
{
  class BoundObserver;

  class BoundedNode :  public AbstractNode
  {
  public:
    BoundedNode() = default;
    BoundedNode(const BoundedNode&) = delete;
    BoundedNode& operator = (const BoundedNode&) = delete;
    virtual ~BoundedNode() noexcept;

    inline const Sphere& localBoundSphere() const noexcept;
    inline const Sphere& transformedBoundSphere() const noexcept;

    void addBoundObserver(BoundObserver& observer);
    void removeBoundObserver(BoundObserver& observer) noexcept;

    virtual void setTransformMatrix(
                                  const glm::mat4& newValue) noexcept override;

  protected:
    virtual void setLocalBoundSphere(const Sphere& newValue) noexcept;

  private:
    void updateTransformedBoundSphere() noexcept;

  private:
    Sphere _localBoundSphere;
    Sphere _transformedBoundSphere;

    using BoundObservers = std::vector<BoundObserver*>;
    BoundObservers _boundObservers;
  };

  inline const Sphere& BoundedNode::localBoundSphere() const noexcept
  {
    return _localBoundSphere;
  }

  inline const Sphere& BoundedNode::transformedBoundSphere() const noexcept
  {
    return _transformedBoundSphere;
  }
}