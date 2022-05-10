#pragma once

#include <vulkan/vulkan.h>

#include <mtt/render/DrawPlan/AbstractAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/render/PlainBuffer.h>

#include <Render/Particles/ParticlesShadowmapTechnique.h>

class ShadowDrawParticlesAction : public mtt::AbstractAction
{
public:
  using NearFarInfo = ParticlesShadowmapTechnique::NearFarInfo;
  using NearFarUniform = ParticlesShadowmapTechnique::NearFarUniform;

public:
  ShadowDrawParticlesAction(
          mtt::GraphicsPipeline& pipeline,
          VkViewport viewport,
          VkRect2D scissor,
          uint32_t pointsNumber,
          mtt::PlainBuffer& indicesBuffer,
          mtt::VolatileUniform<mtt::DrawMatrices>& matricesUniform,
          const mtt::DrawMatrices& drawMatrices,
          NearFarUniform& nearFarUniform,
          NearFarInfo nearfarData,
          mtt::VolatileUniform<mtt::MppxDistanceFunction>& mppxFunctionUniform,
          mtt::MppxDistanceFunction mppxFunctionValues,
          mtt::VolatileUniform<glm::vec2>& falloffUniform,
          glm::vec2 falloffValue);
  ShadowDrawParticlesAction(const ShadowDrawParticlesAction&) = delete;
  ShadowDrawParticlesAction& operator = (
                                    const ShadowDrawParticlesAction&) = delete;
  virtual ~ShadowDrawParticlesAction() noexcept = default;

  virtual void execute(mtt::DrawContext& context) override;

private:
  mtt::GraphicsPipeline& _pipeline;
  VkViewport _viewport;
  VkRect2D _scissor;
  uint32_t _pointsNumber;
  mtt::Ref<mtt::PlainBuffer> _indicesBuffer;
  mtt::VolatileUniform<mtt::DrawMatrices>& _matricesUniform;
  mtt::DrawMatrices _drawMatrices;
  NearFarUniform& _nearFarUniform;
  NearFarInfo _nearfarData;
  mtt::VolatileUniform<mtt::MppxDistanceFunction>& _mppxFunctionUniform;
  mtt::MppxDistanceFunction _mppxFunctionValues;
  mtt::VolatileUniform<glm::vec2>& _falloffUniform;
  glm::vec2 _falloffValue;
};