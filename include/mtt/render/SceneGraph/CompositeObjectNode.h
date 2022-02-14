#pragma once

#include <mtt/render/SceneGraph/ProtectedGroupNode.h>
#include <mtt/render/CompositeRenderObject.h>

namespace mtt
{
  class CompositeObjectNode : public CompositeRenderObject,
                              public ProtectedGroupNode
  {
  public:
    CompositeObjectNode() = default;
    CompositeObjectNode(const CompositeObjectNode&) = delete;
    CompositeObjectNode& operator = (const CompositeObjectNode&) = delete;
    virtual ~CompositeObjectNode() noexcept = default;
  };
}