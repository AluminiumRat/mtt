#pragma once

#include <mtt/render/Drawable/Drawable.h>
#include <mtt/render/SceneGraph/BoundedNode.h>

namespace mtt
{
  struct DrawPlanBuildInfo;

  class DrawableNode :  public BoundedNode,
                        public Drawable
  {
  public:
    DrawableNode();
    DrawableNode(const DrawableNode&) = delete;
    DrawableNode& operator = (const DrawableNode&) = delete;
    virtual ~DrawableNode() noexcept = default;

    virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) override;

    virtual void setTransformMatrix(
                                  const glm::mat4& newValue) noexcept override;

  private:
    glm::mat3 _normalTransformMatrix;
  };
}