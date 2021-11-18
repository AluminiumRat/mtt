#pragma once

#include <mtt/Render/SceneGraph/DrawableNode.h>

namespace mtt
{
  class SimpleDrawableNode : public DrawableNode
  {
  public:
    SimpleDrawableNode() noexcept;
    SimpleDrawableNode(const SimpleDrawableNode&) = delete;
    SimpleDrawableNode& operator = (const SimpleDrawableNode&) = delete;
    virtual ~SimpleDrawableNode() noexcept = default;

    inline const Drawable* drawable() const noexcept;
    inline Drawable* drawable() noexcept;
    virtual void setDrawable( Drawable* newDrawable,
                              const Sphere& localBoundSphere) noexcept;

    inline void setLocalBoundSphere(const Sphere& newValue) noexcept;

  protected:
    virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;
  
  private:
    Drawable* _drawable;
  };

  const Drawable* SimpleDrawableNode::drawable() const noexcept
  {
    return _drawable;
  }

  Drawable* SimpleDrawableNode::drawable() noexcept
  {
    return _drawable;
  }

  void SimpleDrawableNode::setLocalBoundSphere(const Sphere& newValue) noexcept
  {
    DrawableNode::setLocalBoundSphere(newValue);
  }
}