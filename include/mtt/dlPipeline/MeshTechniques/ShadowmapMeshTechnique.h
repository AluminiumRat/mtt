#pragma once

#include <mtt/render/Mesh/BaseGeometryTechnique.h>

namespace mtt
{
  namespace dlPipeline
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
      struct NearFarInfo
      {
        alignas(4) float nearDistance;
        alignas(4) float nearFarDistance;
      };
      using NearFarUniform = VolatileUniform<NearFarInfo>;
      NearFarUniform _nearFarUniform;
    };
  }
}