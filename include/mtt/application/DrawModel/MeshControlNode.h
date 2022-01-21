#pragma once

#include <mtt/application/DrawModel/SkinControlNode.h>
#include <mtt/render/Drawable/LODDrawableFilter.h>
#include <mtt/render/Mesh/Mesh.h>

namespace mtt
{
  class MeshControlNode : public SkinControlNode
  {
  public:
    MeshControlNode(LogicalDevice& device);
    MeshControlNode(const MeshControlNode&) = delete;
    MeshControlNode& operator = (const MeshControlNode&) = delete;
    virtual ~MeshControlNode() noexcept = default;

    inline LODDrawableFilter& lodFilter() noexcept;
    inline const LODDrawableFilter& lodFilter() const noexcept;
    inline Mesh& mesh() noexcept;
    inline const Mesh& mesh() const noexcept;

    virtual void setBoneRefs(const BoneRefs& newValue) override;

  private:
    LODDrawableFilter _lodFilter;
    Mesh _mesh;
  };

  inline LODDrawableFilter& MeshControlNode::lodFilter() noexcept
  {
    return _lodFilter;
  }

  inline const LODDrawableFilter&
                              MeshControlNode::lodFilter() const noexcept
  {
    return _lodFilter;
  }

  inline Mesh& MeshControlNode::mesh() noexcept
  {
    return _mesh;
  }

  inline const Mesh& MeshControlNode::mesh() const noexcept
  {
    return _mesh;
  }
}