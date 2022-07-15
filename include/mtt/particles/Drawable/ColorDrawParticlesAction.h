#pragma once

#include <vulkan/vulkan.h>

#include <mtt/render/DrawPlan/AbstractAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/render/PlainBuffer.h>

namespace mtt
{
  class ColorDrawParticlesAction : public AbstractAction
  {
  public:
    ColorDrawParticlesAction(
                  GraphicsPipeline& pipeline,
                  VkViewport viewport,
                  VkRect2D scissor,
                  uint32_t pointsNumber,
                  PlainBuffer& indicesBuffer,
                  VolatileUniform<DrawMatrices>& matricesUniform,
                  const DrawMatrices& drawMatrices,
                  VolatileUniform<MppxDistanceFunction>& mppxFunctionUniform,
                  MppxDistanceFunction mppxFunctionValues,
                  VolatileUniform<glm::vec2>& falloffUniform,
                  glm::vec2 falloffValue,
                  Texture2D& depthSamplerTexture);
    ColorDrawParticlesAction(const ColorDrawParticlesAction&) = delete;
    ColorDrawParticlesAction& operator = (
                                    const ColorDrawParticlesAction&) = delete;
    virtual ~ColorDrawParticlesAction() noexcept = default;

    virtual void execute(DrawContext& context) override;

  private:
    GraphicsPipeline& _pipeline;
    VkViewport _viewport;
    VkRect2D _scissor;
    uint32_t _pointsNumber;
    Ref<PlainBuffer> _indicesBuffer;
    VolatileUniform<DrawMatrices>& _matricesUniform;
    DrawMatrices _drawMatrices;
    VolatileUniform<MppxDistanceFunction>& _mppxFunctionUniform;
    MppxDistanceFunction _mppxFunctionValues;
    VolatileUniform<glm::vec2>& _falloffUniform;
    glm::vec2 _falloffValue;
    Texture2D& _depthSamplerTexture;
  };
}