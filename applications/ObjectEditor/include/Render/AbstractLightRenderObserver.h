#pragma once

#include <glm/vec3.hpp>

#include <optional>
#include <vector>

#include <mtt/render/Drawable/VisibleDrawableFilter.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

#include <Render/Object3DRenderObserver.h>
#include <Render/IconDrawableNode.h>

namespace mtt
{
  class DrawableNode;
  class AreaModificator;

  namespace clPipeline
  {
    class AbstractLight;
  }
}

class LightObject;

class AbstractLightRenderObserver : public Object3DRenderObserver
{
  Q_OBJECT

public:
  /// If iconFilename is empty or iconSize <= 0 then no icon node will be
  ///  created
  AbstractLightRenderObserver(LightObject& object,
                              EditorCommonData& commonData,
                              const QString& iconFilename,
                              float iconSize);
  AbstractLightRenderObserver(const AbstractLightRenderObserver&) = delete;
  AbstractLightRenderObserver& operator = (
                                  const AbstractLightRenderObserver&) = delete;
  virtual ~AbstractLightRenderObserver() noexcept = default;

protected:
  void setLightObject(mtt::clPipeline::AbstractLight& light);

  inline bool infinityArea() const noexcept;
  void setInfinityArea(bool newValue);

  /// newGeometry should be in VK_PRIMITIVE_TOPOLOGY_LINE_LIST topology
  void setHullGeometry(const std::vector<glm::vec3> newGeometry);

private:
  void _clearDrawables() noexcept;
  void _updateEnabled() noexcept;

private:
  LightObject& _object;
  bool _infinityArea;
  mtt::DrawableNode* _defferedLightApplicator;
  mtt::VisibleDrawableFilter _enableFilter;
  mtt::AreaModificator* _forwardLightApplicator;

  std::optional<IconDrawableNode> _iconNode;

  mtt::Mesh _hullMesh;
  mtt::SimpleDrawableNode _hullNode;
};

inline bool AbstractLightRenderObserver::infinityArea() const noexcept
{
  return _infinityArea;
}
