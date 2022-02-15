#pragma once

#include <mtt/render/Drawable/AutoscaleDrawableModificator.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

#include <Render/Object3DRenderObserver.h>

namespace mtt
{
  class SkeletonObject;
}

class SkeletonRenderObserver : public Object3DRenderObserver
{
  Q_OBJECT

public:
  SkeletonRenderObserver( mtt::SkeletonObject& object,
                          EditorCommonData& commonData);
  SkeletonRenderObserver(const SkeletonRenderObserver&) = delete;
  SkeletonRenderObserver& operator = (const SkeletonRenderObserver&) = delete;
  virtual ~SkeletonRenderObserver() noexcept = default;

private:
  mtt::Mesh _crossMesh;
  mtt::AutoscaleDrawableModificator _autoscale;
  mtt::SimpleDrawableNode _drawableNode;
};