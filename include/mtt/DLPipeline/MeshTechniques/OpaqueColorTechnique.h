#pragma once

#include <mtt/Render/Mesh/BaseGeometryTechnique.h>

namespace mtt
{
  namespace DLPipeline
  {
    class OpaqueColorTechnique : public BaseGeometryTechnique
    {
    public:
      explicit OpaqueColorTechnique(VkPrimitiveTopology topology);
      OpaqueColorTechnique(const OpaqueColorTechnique&) = delete;
      OpaqueColorTechnique& operator = (const OpaqueColorTechnique&) = delete;
      virtual ~OpaqueColorTechnique() noexcept = default;

    protected:
      virtual void adjustPipeline(GraphicsPipeline& pipeline,
                                  AbstractRenderPass& renderPass) override;
      virtual void createRenderAction(
                                    DrawPlanBuildInfo& buildInfo,
                                    GraphicsPipeline& pipeline,
                                    MatricesUniform& matricesUniform) override;
    };
  }
}