#pragma once

#include <optional>

#include <mtt/render/Drawable/BoneMatricesSetter.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

#include <Render/BonesObserver.h>
#include <Render/MaterialObserver.h>
#include <Render/Object3DRenderObserver.h>

class MeshObject;

class MeshRenderObserver : public Object3DRenderObserver
{
  Q_OBJECT

public:
  MeshRenderObserver(MeshObject& object, EditorCommonData& commonData);
  MeshRenderObserver(const MeshRenderObserver&) = delete;
  MeshRenderObserver& operator = (const MeshRenderObserver&) = delete;
  virtual ~MeshRenderObserver() noexcept = default;

private:
  void _updateMesh() noexcept;
  void _updateBones() noexcept;
  void _updateMaterialObserver() noexcept;

private:
  MeshObject& _object;
  mtt::BoneMatricesSetter _boneMatricesSetter;
  mtt::Mesh _mesh;
  mtt::SimpleDrawableNode _drawableNode;
  std::optional<BonesObserver> _bonesObserver;
  std::optional<MaterialObserver> _materialObserver;
};