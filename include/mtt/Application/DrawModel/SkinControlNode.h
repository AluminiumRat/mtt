#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/Render/Drawable/BoneMatricesSetter.h>
#include <mtt/Render/SceneGraph/SimpleDrawableNode.h>

namespace mtt
{
  class Joint;

  class SkinControlNode : public SimpleDrawableNode
  {
  public:
    struct BoneRefData
    {
      const Joint* joint;
      glm::mat4 inverseBoneMatrix;
    };
    using BoneRefs = std::vector<BoneRefData>;

  public:
    SkinControlNode() = default;
    SkinControlNode(const SkinControlNode&) = delete;
    SkinControlNode& operator = (const SkinControlNode&) = delete;
    virtual ~SkinControlNode() noexcept = default;

    virtual void setBoneRefs(const BoneRefs& newValue);
    inline const BoneRefs& boneRefs() const noexcept;

    void updateSkinFromBones();

  private:
    BoneMatricesSetter _boneMatricesSetter;
    BoneRefs _boneRefs;
  };

  inline const SkinControlNode::BoneRefs&
                                      SkinControlNode::boneRefs() const noexcept
  {
    return _boneRefs;
  }
}