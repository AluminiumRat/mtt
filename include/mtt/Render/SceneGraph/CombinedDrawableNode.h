#pragma once

#include <vector>

#include <mtt/Render/SceneGraph/BoundObserver.h>
#include <mtt/Render/SceneGraph/DrawableNode.h>

namespace mtt
{
  class CombinedDrawableNode :  public DrawableNode,
                                public BoundObserver
  {
  public:
    CombinedDrawableNode() noexcept = default;
    CombinedDrawableNode(const CombinedDrawableNode&) = delete;
    CombinedDrawableNode& operator = (const CombinedDrawableNode&) = delete;
    virtual ~CombinedDrawableNode() noexcept;
  
    virtual void addNode(DrawableNode& node);
    virtual void removeNode(DrawableNode& node) noexcept;

  protected:
    virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

    virtual void onNodeBoundChanged(DrawableNode& node,
                                    const Sphere& oldBound) noexcept override;
    virtual void onNodeRemoved(DrawableNode& node) noexcept override;

    /// Translate childs bounding sphere to local bounding sphere.
    /// You must reimpliment this method if your node changes spatial properties
    /// of its childs.
    virtual void updateBoundSphere(Sphere& boundSphere) noexcept;

  private:
    void _updateBound() noexcept;

  private:
    using Drawables = std::vector<DrawableNode*>;
    Drawables _drawables;
  };
}