#pragma once

#include <mtt/render/Mesh/BaseGeometryTechnique.h>

namespace mtt
{
  namespace clPipeline
  {
    class EmissionTechnique : public BaseGeometryTechnique
    {
    public:
      explicit EmissionTechnique(VkPrimitiveTopology topology);
      EmissionTechnique(const EmissionTechnique&) = delete;
      EmissionTechnique& operator = (const EmissionTechnique&) = delete;
      virtual ~EmissionTechnique() noexcept = default;

     protected:
      virtual void adjustPipeline(GraphicsPipeline& pipeline,
                                  AbstractRenderPass& renderPass) override;
      virtual void createRenderAction(DrawPlanBuildInfo& buildInfo,
                                      GraphicsPipeline& pipeline,
                                    MatricesUniform& matricesUniform) override;
    };
  }
}