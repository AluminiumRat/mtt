#pragma once

#include <mtt/render/SceneGraph/AreaModificator.h>

#include <Render/Particles/ParticlesAbstractTechnique.h>

class ParticlesColorTechnique : public ParticlesAbstractTechnique,
                                private mtt::AreaModificator::Consumer
{
public:
  ParticlesColorTechnique(ParticlesDrawCommonData& commonData,
                          mtt::AreaModificatorSet& modificatorSet);
  ParticlesColorTechnique(const ParticlesColorTechnique&) = delete;
  ParticlesColorTechnique& operator = (const ParticlesColorTechnique&) = delete;
  virtual ~ParticlesColorTechnique() noexcept;

  virtual void resetPipeline() noexcept override;

protected:
  virtual void adjustPipeline(mtt::GraphicsPipeline& pipeline) override;
  virtual void buildDrawAction(
            mtt::DrawBin& renderBin,
            mtt::DrawPlanBuildInfo& buildInfo,
            mtt::GraphicsPipeline& pipeline,
            uint32_t pointsNumber,
            mtt::PlainBuffer& indices,
            mtt::VolatileUniform<mtt::DrawMatrices>& matricesUniform) override;

private:
  void _applyAreaModifictors( mtt::ShaderModule& shader,
                              mtt::GraphicsPipeline& pipeline);

private:
  mtt::AreaModificatorSet& _modificatorSet;
};
