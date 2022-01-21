#pragma once

#include <memory>
#include <vector>

#include <mtt/render/Drawable/LODDrawableFilter.h>
#include <mtt/render/Drawable/DrawableList.h>
#include <mtt/render/SceneGraph/CombinedDrawableNode.h>

#include <Render/AbstractObjectRenderObserver.h>
#include <Render/MeshRenderObserver.h>

class LODObject;
class MeshObject;

class LODRenderObserver : public AbstractObjectRenderObserver
{
  Q_OBJECT

public:
  LODRenderObserver(LODObject& object, EditorCommonData& commonData);
  LODRenderObserver(const LODRenderObserver&) = delete;
  LODRenderObserver& operator = (const LODRenderObserver&) = delete;
  virtual ~LODRenderObserver() noexcept = default;

private:
  void _addGeometry(MeshObject& object) noexcept;
  void _removeGeometry(MeshObject& object) noexcept;
  void _updateRange() noexcept;
  void _addCulled(mtt::DrawableNode& drawable) noexcept;
  void _removeCulled(mtt::DrawableNode& drawable) noexcept;
  void _addUnculled(mtt::Drawable& drawable) noexcept;
  void _removeUnculled(mtt::Drawable& drawable) noexcept;

private:
  LODObject& _object;

  using Subobservers = std::vector<std::unique_ptr<MeshRenderObserver>>;
  Subobservers _subobservers;

  mtt::LODDrawableFilter _lodFilter;
  mtt::CombinedDrawableNode _culledDrawable;
  mtt::DrawableList _unculledDrawable;
};