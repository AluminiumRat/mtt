#pragma once

#include <vector>

#include <mtt/render/Drawable/DrawableList.h>
#include <mtt/render/SceneGraph/BoundObserver.h>
#include <mtt/render/SceneGraph/DrawableNode.h>

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

    virtual void registerAreaModificators(AreaModificatorSet& set) override;
    virtual void unregisterAreaModificators(
                                    AreaModificatorSet& set) noexcept override;

  protected:
    virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

    virtual void onNodeBoundChanged(BoundedNode& node,
                                    const Sphere& oldBound) noexcept override;
    virtual void onNodeRemoved(BoundedNode& node) noexcept override;

    /// Translate childs bounding sphere to local bounding sphere.
    /// You must reimpliment this method if your node changes spatial properties
    /// of its childs.
    virtual void updateBoundSphere(Sphere& boundSphere) noexcept;

  private:
    void _updateBound() noexcept;

  private:
    DrawableList _drawables;
  };
}