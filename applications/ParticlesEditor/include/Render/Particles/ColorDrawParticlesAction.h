#pragma once

#include <vulkan/vulkan.h>

#include <mtt/render/DrawPlan/AbstractAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/render/PlainBuffer.h>

class ColorDrawParticlesAction : public mtt::AbstractAction
{
public:
  ColorDrawParticlesAction(
          mtt::GraphicsPipeline& pipeline,
          VkViewport viewport,
          VkRect2D scissor,
          uint32_t pointsNumber,
          mtt::PlainBuffer& indicesBuffer,
          mtt::VolatileUniform<mtt::DrawMatrices>& matricesUniform,
          const mtt::DrawMatrices& drawMatrices,
          mtt::VolatileUniform<mtt::MppxDistanceFunction>& mppxFunctionUniform,
          mtt::MppxDistanceFunction mppxFunctionValues,
          mtt::VolatileUniform<glm::vec2>& falloffUniform,
          glm::vec2 falloffValue,
          mtt::Texture2D& depthSamplerTexture);
  ColorDrawParticlesAction(const ColorDrawParticlesAction&) = delete;
  ColorDrawParticlesAction& operator = (
                                      const ColorDrawParticlesAction&) = delete;
  virtual ~ColorDrawParticlesAction() noexcept = default;

  virtual void execute(mtt::DrawContext& context) override;

private:
  mtt::GraphicsPipeline& _pipeline;
  VkViewport _viewport;
  VkRect2D _scissor;
  uint32_t _pointsNumber;
  mtt::Ref<mtt::PlainBuffer> _indicesBuffer;
  mtt::VolatileUniform<mtt::DrawMatrices>& _matricesUniform;
  mtt::DrawMatrices _drawMatrices;
  mtt::VolatileUniform<mtt::MppxDistanceFunction>& _mppxFunctionUniform;
  mtt::MppxDistanceFunction _mppxFunctionValues;
  mtt::VolatileUniform<glm::vec2>& _falloffUniform;
  glm::vec2 _falloffValue;
  mtt::Texture2D& _depthSamplerTexture;
};