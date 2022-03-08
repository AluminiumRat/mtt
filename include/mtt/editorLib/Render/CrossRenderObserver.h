#pragma once

#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/render/Drawable/AutoscaleDrawableModificator.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

namespace mtt
{
  class Object3D;

  class CrossRenderObserver : public Object3DRenderObserver
  {
    Q_OBJECT

  public:
    CrossRenderObserver(Object3D& object,
                        CommonEditData& commonData);
    CrossRenderObserver(const CrossRenderObserver&) = delete;
    CrossRenderObserver& operator = (const CrossRenderObserver&) = delete;
    virtual ~CrossRenderObserver() noexcept = default;

  private:
    Mesh _crossMesh;
    AutoscaleDrawableModificator _autoscale;
    SimpleDrawableNode _drawableNode;
  };
}