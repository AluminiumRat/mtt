#pragma once

#include <mtt/Render/SceneGraph/AbstractField.h>
#include <mtt/Utilities/Abort.h>

namespace mtt
{
  class VisitedField : public AbstractField
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
    VisitedField() noexcept = default;
    VisitedField(const VisitedField&) = delete;
    VisitedField& operator = (const VisitedField&) = delete;
    virtual ~VisitedField() noexcept = default;

    inline void pass(Visitor& visitor, const ViewFrustum& frustum) const;

  protected:
    virtual void enumerateAreas(Visitor& visitor,
                                const ViewFrustum& frustum) const = 0;
  };

  inline void VisitedField::Area::addNode(DrawableNode& node)
  {
    nodes.push_back(&node);
  }

  inline void VisitedField::Area::removeNode(DrawableNode& node) noexcept
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

    Abort("AbstractField::Area::removeNode: the node is not found.");
  }

  inline void VisitedField::pass( Visitor& visitor,
                                  const ViewFrustum& frustum) const
  {
    visitor.startPass();
    enumerateAreas(visitor, frustum);
    visitor.finishPass();
  }
};