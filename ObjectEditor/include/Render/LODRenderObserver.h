#pragma once

#include <memory>
#include <vector>

#include <mtt/Render/Drawable/LODDrawableFilter.h>
#include <mtt/Render/Drawable/DrawableList.h>
#include <mtt/Render/SceneGraph/CombinedDrawableNode.h>

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

  mtt::LODDrawableFilter _lodFilter;
  mtt::CombinedDrawableNode _culledDrawable;
  mtt::DrawableList _unculledDrawable;

  using Subobservers = std::vector<std::unique_ptr<MeshRenderObserver>>;
  Subobservers _subobservers;
};