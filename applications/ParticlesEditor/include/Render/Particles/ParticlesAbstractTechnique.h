#pragma once

#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Mesh/AbstractMeshTechnique.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

#include <Render/Particles/ParticlesDrawCommonData.h>

namespace mtt
{
  class AbstractRenderPass;
}

class ParticlesAbstractTechnique : public mtt::AbstractMeshTechnique
{
public:
  ParticlesAbstractTechnique( ParticlesDrawCommonData& commonData,
                              mtt::StageIndex stage);
  ParticlesAbstractTechnique(const ParticlesAbstractTechnique&) = delete;
  ParticlesAbstractTechnique& operator = (
                                    const ParticlesAbstractTechnique&) = delete;
  virtual ~ParticlesAbstractTechnique() noexcept = default;

  inline mtt::StageIndex stage() const noexcept;

  void clearPipeline() noexcept;
  virtual void addToDrawPlan(mtt::DrawPlanBuildInfo& buildInfo) override;

protected:
  inline ParticlesDrawCommonData& commonData() const noexcept;

  virtual void adjustPipeline(mtt::GraphicsPipeline& pipeline) = 0;
  virtual void buildDrawAction(
                  mtt::DrawBin& renderBin,
                  mtt::DrawPlanBuildInfo& buildInfo,
                  mtt::GraphicsPipeline& pipeline,
                  uint32_t pointsNumber,
                  mtt::PlainBuffer& indices,
                  mtt::VolatileUniform<mtt::DrawMatrices>& matricesUniform) = 0;

private:
  std::string _makeTextureExtentDefine() const;
  void _rebuildPipeline(mtt::AbstractRenderPass& renderPass);
  mtt::Ref<mtt::PlainBuffer> _makeIndices(
                              const mtt::DrawPlanBuildInfo& buildInfo,
                              mtt::LogicalDevice& device) const;

private:
  ParticlesDrawCommonData& _commonData;
  mtt::StageIndex _stage;
  mtt::VolatileUniform<mtt::DrawMatrices> _matricesUniform;
  std::optional<mtt::GraphicsPipeline> _pipeline;
};

inline ParticlesDrawCommonData&
                        ParticlesAbstractTechnique::commonData() const noexcept
{
  return _commonData;
}

inline mtt::StageIndex ParticlesAbstractTechnique::stage() const noexcept
{
  return _stage;
}
