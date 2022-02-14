#pragma once

#include <vector>

#include <mtt/render/SceneGraph/AbstractNode.h>

namespace mtt
{
  class ProtectedGroupNode : public AbstractNode
  {
  public:
    ProtectedGroupNode() noexcept = default;
    ProtectedGroupNode(const ProtectedGroupNode&) = delete;
    ProtectedGroupNode& operator = (const ProtectedGroupNode&) = delete;
    virtual ~ProtectedGroupNode() noexcept = default;

    /// Only translation, rotation and uniform scale.
    /// Nonuniform scale is not supported.
    virtual void setTransformMatrix(
                                  const glm::mat4& newValue) noexcept override;

    inline size_t childNumber() const noexcept;
    inline AbstractNode& child(size_t index) noexcept;
    inline const AbstractNode& child(size_t index) const noexcept;
    bool hasChild(AbstractNode& child) const noexcept;

  protected:
    virtual void addChildProtected(AbstractNode& child);
    virtual void removeChildProtected(size_t index) noexcept;
    void removeChildProtected(AbstractNode& child) noexcept;

  private:
    using Childs = std::vector<AbstractNode*>;
    Childs _childs;
  };

  inline size_t ProtectedGroupNode::childNumber() const noexcept
  {
    return _childs.size();
  }

  inline AbstractNode& ProtectedGroupNode::child(size_t index) noexcept
  {
    return *_childs[index];
  }

  inline const AbstractNode&
                          ProtectedGroupNode::child(size_t index) const noexcept
  {
    return *_childs[index];
  }
}