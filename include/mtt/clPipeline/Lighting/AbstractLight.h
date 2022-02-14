#pragma once

#include <glm/vec3.hpp>

#include <mtt/render/SceneGraph/ProtectedGroupNode.h>
#include <mtt/render/CompositeRenderObject.h>

namespace mtt
{
  class AreaModificator;
  class DrawableNode;

  namespace clPipeline
  {
    class AbstractLight : public CompositeRenderObject,
                          public ProtectedGroupNode
    {
    public:
      AbstractLight() = default;
      AbstractLight(const AbstractLight&) = delete;
      AbstractLight& operator = (const AbstractLight&) = delete;
      virtual ~AbstractLight() noexcept = default;

      virtual DrawableNode* defferedLightApplicator() noexcept = 0;
      virtual AreaModificator* forwardLightModificator() noexcept = 0;
    };
  }
}