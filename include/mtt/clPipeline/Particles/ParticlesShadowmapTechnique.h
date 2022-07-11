#pragma once

#include <mtt/clPipeline/Particles/ParticlesAbstractTechnique.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  namespace clPipeline
  {
    class ParticlesShadowmapTechnique : public ParticlesAbstractTechnique
    {
    public:
      using MaxDistanceUniform = VolatileUniform<float>;

    public:
      ParticlesShadowmapTechnique(ParticlesDrawCommonData& commonData);
      ParticlesShadowmapTechnique(const ParticlesShadowmapTechnique&) = delete;
      ParticlesShadowmapTechnique& operator = (
                                  const ParticlesShadowmapTechnique&) = delete;
      virtual ~ParticlesShadowmapTechnique() noexcept = default;

    protected:
      virtual void adjustPipeline(GraphicsPipeline& pipeline) override;

      virtual void buildDrawAction(
                    DrawBin& renderBin,
                    DrawPlanBuildInfo& buildInfo,
                    GraphicsPipeline& pipeline,
                    uint32_t pointsNumber,
                    PlainBuffer& indices,
                    VolatileUniform<DrawMatrices>& matricesUniform,
                    VolatileUniform<MppxDistanceFunction>& mppxFunctionUniform,
                    VolatileUniform<glm::vec2>& falloffUniform,
                    glm::vec2 falloffValue) override;

    private:
      MaxDistanceUniform _maxDistanceUniform;
    };
  }
}