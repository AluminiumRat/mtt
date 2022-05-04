#pragma once

#include <mtt/render/Pipeline/VolatileUniform.h>

#include <Render/Particles/ParticlesAbstractTechnique.h>

class ParticlesShadowmapTechnique : public ParticlesAbstractTechnique
{
public:
  struct NearFarInfo
  {
    alignas(4) float nearDistance;
    alignas(4) float nearFarDistance;
  };
  using NearFarUniform = mtt::VolatileUniform<NearFarInfo>;

public:
  ParticlesShadowmapTechnique(ParticlesDrawCommonData& commonData);
  ParticlesShadowmapTechnique(const ParticlesShadowmapTechnique&) = delete;
  ParticlesShadowmapTechnique& operator = (
                                  const ParticlesShadowmapTechnique&) = delete;
  virtual ~ParticlesShadowmapTechnique() noexcept = default;

protected:
  virtual void adjustPipeline(mtt::GraphicsPipeline& pipeline) override;

  virtual void buildDrawAction(
            mtt::DrawBin& renderBin,
            mtt::DrawPlanBuildInfo& buildInfo,
            mtt::GraphicsPipeline& pipeline,
            uint32_t pointsNumber,
            mtt::PlainBuffer& indices,
            mtt::VolatileUniform<mtt::DrawMatrices>& matricesUniform,
            mtt::VolatileUniform<glm::vec2>& falloffUniform,
            glm::vec2 falloffValue) override;

private:
  NearFarUniform _nearFarUniform;
};
