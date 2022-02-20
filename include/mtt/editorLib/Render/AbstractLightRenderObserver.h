#pragma once

#include <glm/vec3.hpp>

#include <optional>
#include <vector>

#include <mtt/editorLib/Render/Object3DRenderObserver.h>
#include <mtt/editorLib/Render/IconDrawableNode.h>
#include <mtt/render/Drawable/VisibleDrawableFilter.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>

namespace mtt
{
  class AreaModificator;
  class CompositeObjectNode;
  class DrawableNode;
  class LightObject;

  class AbstractLightRenderObserver : public Object3DRenderObserver
  {
    Q_OBJECT

  public:
    /// If iconFilename is empty or iconSize <= 0 then no icon node will be
    /// created
    AbstractLightRenderObserver(LightObject& object,
                                CommonEditData& commonData,
                                const QString& iconFilename,
                                float iconSize);
    AbstractLightRenderObserver(const AbstractLightRenderObserver&) = delete;
    AbstractLightRenderObserver& operator = (
                                  const AbstractLightRenderObserver&) = delete;
    virtual ~AbstractLightRenderObserver() noexcept = default;

  protected:
    void setLightObject(CompositeObjectNode& light);

    /// newGeometry should be in VK_PRIMITIVE_TOPOLOGY_LINE_LIST topology
    void setHullGeometry(const std::vector<glm::vec3> newGeometry);

  private:
    void _clearLightRenderer() noexcept;
    void _updateEnabled() noexcept;

  private:
    LightObject& _object;

    CompositeObjectNode* _lightRenderer;

    std::optional<IconDrawableNode> _iconNode;

    Mesh _hullMesh;
    SimpleDrawableNode _hullNode;
  };
}