#pragma once

#include <mtt/Render/Drawable/BoneMatricesSetter.h>
#include <mtt/Render/Drawable/LODDrawableFilter.h>
#include <mtt/Render/Mesh/Mesh.h>
#include <mtt/Render/SceneGraph/SimpleDrawableNode.h>

namespace mtt
{
  class Joint;

  class DrawModelMeshNode : public SimpleDrawableNode
  {
  public:
    struct BoneRefData
    {
      const Joint* joint;
      glm::mat4 inverseBoneMatrix;
    };
    using BoneRefs = std::vector<BoneRefData>;

  public:
    DrawModelMeshNode(LogicalDevice& device);
    DrawModelMeshNode(const DrawModelMeshNode&) = delete;
    DrawModelMeshNode& operator = (const DrawModelMeshNode&) = delete;
    virtual ~DrawModelMeshNode() noexcept = default;

    inline LODDrawableFilter& lodFilter() noexcept;
    inline const LODDrawableFilter& lodFilter() const noexcept;
    inline Mesh& mesh() noexcept;
    inline const Mesh& mesh() const noexcept;

    void setBoneRefs(const BoneRefs& newValue);
    inline const BoneRefs& boneRefs() const noexcept;

    void updateSkinFromBones();

  private:
    LODDrawableFilter _lodFilter;
    BoneMatricesSetter _boneMatricesSetter;
    Mesh _mesh;
    BoneRefs _boneRefs;
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

  inline const DrawModelMeshNode::BoneRefs&
                                    DrawModelMeshNode::boneRefs() const noexcept
  {
    return _boneRefs;
  }
}