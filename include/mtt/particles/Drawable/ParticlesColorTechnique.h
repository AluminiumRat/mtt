#pragma once

#include <mtt/particles/Drawable/ParticlesAbstractTechnique.h>
#include <mtt/render/SceneGraph/AreaModificator.h>

namespace mtt
{
  class ParticlesColorTechnique : public ParticlesAbstractTechnique,
                                  private AreaModificator::Consumer
  {
  public:
    ParticlesColorTechnique(ParticlesDrawCommonData& commonData,
                            AreaModificatorSet& modificatorSet);
    ParticlesColorTechnique(const ParticlesColorTechnique&) = delete;
    ParticlesColorTechnique& operator = (
                                      const ParticlesColorTechnique&) = delete;
    virtual ~ParticlesColorTechnique() noexcept;

    virtual void resetPipeline() noexcept override;

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
    void _applyAreaModifictors( ShaderModule& shader,
                                GraphicsPipeline& pipeline);

  private:
    AreaModificatorSet& _modificatorSet;
  };
}