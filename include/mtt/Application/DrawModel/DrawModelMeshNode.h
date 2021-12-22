#pragma once

#include <mtt/Render/Drawable/LODDrawableFilter.h>
#include <mtt/Render/Mesh/Mesh.h>
#include <mtt/Render/SceneGraph/SimpleDrawableNode.h>

namespace mtt
{
  class DrawModelMeshNode : public SimpleDrawableNode
  {
  public:
    DrawModelMeshNode(LogicalDevice& device);
    DrawModelMeshNode(const DrawModelMeshNode&) = delete;
    DrawModelMeshNode& operator = (const DrawModelMeshNode&) = delete;
    virtual ~DrawModelMeshNode() noexcept = default;

    inline LODDrawableFilter& lodFilter() noexcept;
    inline const LODDrawableFilter& lodFilter() const noexcept;
    inline Mesh& mesh() noexcept;
    inline const Mesh& mesh() const noexcept;

  private:
    LODDrawableFilter _lodFilter;
    Mesh _mesh;
  };

  inline LODDrawableFilter& DrawModelMeshNode::lodFilter() noexcept
  {
    return _lodFilter;
  }

  inline const LODDrawableFilter& DrawModelMeshNode::lodFilter() const noexcept
  {
    return _lodFilter;
  }

  inline Mesh& DrawModelMeshNode::mesh() noexcept
  {
    return _mesh;
  }

  inline const Mesh& DrawModelMeshNode::mesh() const noexcept
  {
    return _mesh;
  }
}