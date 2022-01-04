#pragma once

#include <mtt/Render/Drawable/AutoscaleDrawableModificator.h>
#include <mtt/Render/Mesh/Mesh.h>
#include <mtt/Render/SceneGraph/SimpleDrawableNode.h>

#include <Render/Object3DRenderObserver.h>

class SkeletonObject;

class SkeletonRenderObserver : public Object3DRenderObserver
{
  Q_OBJECT

public:
  SkeletonRenderObserver(SkeletonObject& object, EditorCommonData& commonData);
  SkeletonRenderObserver(const SkeletonRenderObserver&) = delete;
  SkeletonRenderObserver& operator = (const SkeletonRenderObserver&) = delete;
  virtual ~SkeletonRenderObserver() noexcept = default;

private:
  mtt::Mesh _crossMesh;
  mtt::AutoscaleDrawableModificator _autoscale;
  mtt::SimpleDrawableNode _drawableNode;
};