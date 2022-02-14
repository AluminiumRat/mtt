#pragma once

#include <vector>

#include <mtt/render/SceneGraph/ProtectedGroupNode.h>

namespace mtt
{
  class GroupNode : public ProtectedGroupNode
  {
  public:
    GroupNode() noexcept = default;
    GroupNode(const GroupNode&) = delete;
    GroupNode& operator = (const GroupNode&) = delete;
    virtual ~GroupNode() noexcept = default;

    inline void addChild(AbstractNode& child);
    inline void removeChild(size_t index) noexcept;
    inline void removeChild(AbstractNode& child) noexcept;

  private:
    using Childs = std::vector<AbstractNode*>;
    Childs _childs;
  };

  inline void GroupNode::addChild(AbstractNode& child)
  {
    addChildProtected(child);
  }

  inline void GroupNode::removeChild(size_t index) noexcept
  {
    removeChildProtected(index);
  }

  inline void GroupNode::removeChild(AbstractNode& child) noexcept
  {
    removeChildProtected(child);
  }
}