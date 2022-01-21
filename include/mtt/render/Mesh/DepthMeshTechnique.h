#pragma once

#include <mtt/render/Mesh/BaseGeometryTechnique.h>

namespace mtt
{
  /// Draws  object only to depth buffer. FramePlan::DEPTH_MAP_STAGE is used.
  /// Only the position buffer is required.
  class DepthMeshTechnique : public BaseGeometryTechnique
  {
  public:
    explicit DepthMeshTechnique(VkPrimitiveTopology topology);
    DepthMeshTechnique(const DepthMeshTechnique&) = delete;
    DepthMeshTechnique& operator = (const DepthMeshTechnique&) = delete;
    virtual ~DepthMeshTechnique()  noexcept = default;

  protected:
    virtual void adjustPipeline(GraphicsPipeline& pipeline,
                                AbstractRenderPass& renderPass) override;
  };
}