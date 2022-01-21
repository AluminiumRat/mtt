#pragma once

#include <mtt/render/SceneGraph/GroupNode.h>

namespace mtt
{
  class Joint : public GroupNode
  {
  public:
    Joint() noexcept;
    Joint(const Joint&) = delete;
    Joint& operator = (const Joint&) = delete;
    virtual ~Joint() noexcept = default;

    /// Only translation, rotation and uniform scale.
    /// Nonuniform scale is not supported.
    virtual void setTransformMatrix(
                                  const glm::mat4& newValue) noexcept override;

    inline const glm::mat4& jointMatrix() const noexcept;
  
    /// Only translation, rotation and uniform scale.
    /// Nonuniform scale is not supported.
    virtual void setJointMatrix(const glm::mat4& newValue) noexcept;

    /// Set joint matrix without updating the child. Used for deferred update
    /// via setTransformMatrix.
    /// Only translation, rotation and uniform scale.
    /// Nonuniform scale is not supported.
    virtual void silentSetJointMatrix(const glm::mat4& newValue) noexcept;

    virtual void addChild(AbstractNode& child) override;

  private:
    void _updateChilds() noexcept;
  
  private:
    glm::mat4 _jointMatrix;
  };

  inline const glm::mat4& Joint::jointMatrix() const noexcept
  {
    return _jointMatrix;
  }
}