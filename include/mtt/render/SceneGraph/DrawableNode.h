#pragma once

#include <vector>

#include <glm/mat4x4.hpp>

#include <mtt/render/Drawable/Drawable.h>
#include <mtt/render/SceneGraph/AbstractNode.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/Sphere.h>

namespace mtt
{
  class BoundObserver;
  struct DrawPlanBuildInfo;

  class DrawableNode :  public AbstractNode,
                        public Drawable
  {
  public:
    DrawableNode() noexcept;
    DrawableNode(const DrawableNode&) = delete;
    DrawableNode& operator = (const DrawableNode&) = delete;
    virtual ~DrawableNode() noexcept;

    inline const Sphere& localBoundSphere() const noexcept;
    inline const Sphere& transformedBoundSphere() const noexcept;

    void addBoundObserver(BoundObserver& observer);
    void removeBoundObserver(BoundObserver& observer) noexcept;

    virtual void setTransformMatrix(
                                  const glm::mat4& newValue) noexcept override;

    virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) override;

  protected:
    virtual void setLocalBoundSphere(const Sphere& newValue) noexcept;

  private:
    void updateTransformedBoundSphere() noexcept;

  private:
    Sphere _localBoundSphere;
    Sphere _transformedBoundSphere;

    glm::mat3 _normalTransformMatrix;

    using BoundObservers = std::vector<BoundObserver*>;
    BoundObservers _boundObservers;
  };

  inline const Sphere& DrawableNode::localBoundSphere() const noexcept
  {
    return _localBoundSphere;
  }

  inline const Sphere& DrawableNode::transformedBoundSphere() const noexcept
  {
    return _transformedBoundSphere;
  }
}