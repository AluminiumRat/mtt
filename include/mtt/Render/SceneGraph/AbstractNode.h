#pragma once

#include <glm/mat4x4.hpp>

namespace mtt
{
  class AbstractNode
  {
  public:
    inline AbstractNode() noexcept;
    AbstractNode(const AbstractNode&) = delete;
    AbstractNode& operator = (const AbstractNode&) = delete;
    virtual ~AbstractNode() noexcept = default;

    inline const glm::mat4& transformMatrix() const noexcept;

    /// Only translation, rotation and uniform scale.
    /// Nonuniform scale is not supported.
    inline virtual void setTransformMatrix(const glm::mat4& newValue) noexcept;

  private:
    glm::mat4 _transformMatrix;
  };

  inline AbstractNode::AbstractNode() noexcept :
    _transformMatrix(1)
  {
  }

  inline const glm::mat4& AbstractNode::transformMatrix() const noexcept
  {
    return _transformMatrix;
  }

  inline void AbstractNode::setTransformMatrix(
                                              const glm::mat4& newValue) noexcept
  {
    _transformMatrix = newValue;
  }
}