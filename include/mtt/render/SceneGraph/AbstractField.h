#pragma once

#include <vector>

namespace mtt
{
  class DrawableNode;
  class ViewFrustum;

  class AbstractField
  {
  public:
    struct Area
    {
      using Nodes = std::vector<DrawableNode*>;
      Nodes nodes;

      inline void addNode(DrawableNode& node);
      inline void removeNode(DrawableNode& node) noexcept;
    };

    class Visitor
    {
    public:
      virtual void startPass() = 0;
      virtual void visit(const Area&) = 0;
      virtual void finishPass() = 0;
    };

  public:
    AbstractField() noexcept = default;
    AbstractField(const AbstractField&) = delete;
    AbstractField& operator = (const AbstractField&) = delete;
    virtual ~AbstractField() noexcept = default;

    virtual void addNode(DrawableNode& node) = 0;
    virtual void removeNode(DrawableNode& node) noexcept = 0;

    inline void pass(Visitor& visitor, const ViewFrustum& frustum) const;

  protected:
    virtual void enumerateAreas(Visitor& visitor,
                                const ViewFrustum& frustum) const = 0;
  };

  inline void AbstractField::Area::addNode(DrawableNode& node)
  {
    nodes.push_back(&node);
  }

  inline void AbstractField::Area::removeNode(DrawableNode& node) noexcept
  {
    for(Nodes::iterator iNode = nodes.begin();
        iNode != nodes.end();
        iNode++)
    {
      if(*iNode == &node)
      {
        nodes.erase(iNode);
        return;
      }
    }
  }

  inline void AbstractField::pass(Visitor& visitor,
                                  const ViewFrustum& frustum) const
  {
    visitor.startPass();
    enumerateAreas(visitor, frustum);
    visitor.finishPass();
  }
};