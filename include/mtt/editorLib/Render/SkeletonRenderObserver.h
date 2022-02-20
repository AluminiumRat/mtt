#pragma once

#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/render/Drawable/AutoscaleDrawableModificator.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

namespace mtt
{
  class SkeletonObject;

  class SkeletonRenderObserver : public Object3DRenderObserver
  {
    Q_OBJECT

  public:
    SkeletonRenderObserver( SkeletonObject& object,
                            CommonEditData& commonData);
    SkeletonRenderObserver(const SkeletonRenderObserver&) = delete;
    SkeletonRenderObserver& operator = (const SkeletonRenderObserver&) = delete;
    virtual ~SkeletonRenderObserver() noexcept = default;

  private:
    Mesh _crossMesh;
    AutoscaleDrawableModificator _autoscale;
    SimpleDrawableNode _drawableNode;
  };
}