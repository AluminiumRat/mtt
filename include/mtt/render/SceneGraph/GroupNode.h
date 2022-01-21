#pragma once

#include <vector>

#include <mtt/render/SceneGraph/AbstractNode.h>

namespace mtt
{
  class GroupNode : public AbstractNode
  {
  public:
    GroupNode() noexcept = default;
    GroupNode(const GroupNode&) = delete;
    GroupNode& operator = (const GroupNode&) = delete;
    virtual ~GroupNode() noexcept = default;

    /// Only translation, rotation and uniform scale.
    /// Nonuniform scale is not supported.
    virtual void setTransformMatrix(
                                  const glm::mat4& newValue) noexcept override;

    inline size_t childNumber() const noexcept;
    inline AbstractNode& child(size_t index) noexcept;
    inline const AbstractNode& child(size_t index) const noexcept;
    bool hasChild(AbstractNode& child) const noexcept;
    virtual void addChild(AbstractNode& child);
    virtual void removeChild(size_t index) noexcept;
    virtual void removeChild(AbstractNode& child) noexcept;

  private:
    using Childs = std::vector<AbstractNode*>;
    Childs _childs;
  };

  inline size_t GroupNode::childNumber() const noexcept
  {
    return _childs.size();
  }

  inline AbstractNode& GroupNode::child(size_t index) noexcept
  {
    return *_childs[index];
  }

  inline const AbstractNode& GroupNode::child(size_t index) const noexcept
  {
    return *_childs[index];
  }
}