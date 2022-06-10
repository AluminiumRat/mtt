#pragma once

#include <mtt/render/Mesh/BaseGeometryTechnique.h>

namespace mtt
{
  namespace clPipeline
  {
    class ShadowmapMeshTechnique : public BaseGeometryTechnique
    {
    public:
      ShadowmapMeshTechnique(VkPrimitiveTopology topology);
      ShadowmapMeshTechnique(const ShadowmapMeshTechnique&) = delete;
      ShadowmapMeshTechnique& operator = (
                                        const ShadowmapMeshTechnique&) = delete;
      virtual ~ShadowmapMeshTechnique()  noexcept = default;

    protected:
      virtual void adjustPipeline(GraphicsPipeline& pipeline,
                                  AbstractRenderPass& renderPass) override;
      virtual void createRenderAction(
                                    DrawPlanBuildInfo& buildInfo,
                                    GraphicsPipeline& pipeline,
                                    MatricesUniform& matricesUniform) override;
    private:
      VolatileUniform<float> _maxDistanceUniform;
    };
  }
}