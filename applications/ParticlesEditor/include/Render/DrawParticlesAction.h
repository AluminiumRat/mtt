#pragma once

#include <vulkan/vulkan.h>

#include <mtt/render/DrawPlan/AbstractAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/render/PlainBuffer.h>

class DrawParticleAction : public mtt::AbstractAction
{
public:
  DrawParticleAction( mtt::GraphicsPipeline& pipeline,
                      VkViewport viewport,
                      VkRect2D scissor,
                      uint32_t pointsNumber,
                      mtt::PlainBuffer& indicesBuffer,
                      mtt::VolatileUniform<mtt::DrawMatrices>& matricesUniform,
                      const mtt::DrawMatrices& drawMatrices,
                      mtt::Texture2D& depthSamplerTexture);
  DrawParticleAction(const DrawParticleAction&) = delete;
  DrawParticleAction& operator = (const DrawParticleAction&) = delete;
  virtual ~DrawParticleAction() noexcept = default;

  virtual void execute(mtt::DrawContext& context) override;

private:
  mtt::GraphicsPipeline& _pipeline;
  VkViewport _viewport;
  VkRect2D _scissor;
  uint32_t _pointsNumber;
  mtt::Ref<mtt::PlainBuffer> _indicesBuffer;
  mtt::VolatileUniform<mtt::DrawMatrices>& _matricesUniform;
  mtt::DrawMatrices _drawMatrices;
  mtt::Texture2D& _depthSamplerTexture;
};