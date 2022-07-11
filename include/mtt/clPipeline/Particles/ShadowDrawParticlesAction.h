#pragma once

#include <vulkan/vulkan.h>

#include <mtt/clPipeline/Particles/ParticlesShadowmapTechnique.h>
#include <mtt/render/DrawPlan/AbstractAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/render/PlainBuffer.h>

namespace mtt
{
  namespace clPipeline
  {
    class ShadowDrawParticlesAction : public AbstractAction
    {
    public:
      using MaxDistanceUniform = ParticlesShadowmapTechnique::MaxDistanceUniform;

    public:
      ShadowDrawParticlesAction(
                    GraphicsPipeline& pipeline,
                    VkViewport viewport,
                    VkRect2D scissor,
                    uint32_t pointsNumber,
                    PlainBuffer& indicesBuffer,
                    VolatileUniform<DrawMatrices>& matricesUniform,
                    const DrawMatrices& drawMatrices,
                    MaxDistanceUniform& maxDistanceUniform,
                    float maxDistance,
                    VolatileUniform<MppxDistanceFunction>& mppxFunctionUniform,
                    MppxDistanceFunction mppxFunctionValues,
                    VolatileUniform<glm::vec2>& falloffUniform,
                    glm::vec2 falloffValue);
      ShadowDrawParticlesAction(const ShadowDrawParticlesAction&) = delete;
      ShadowDrawParticlesAction& operator = (
                                    const ShadowDrawParticlesAction&) = delete;
      virtual ~ShadowDrawParticlesAction() noexcept = default;

      virtual void execute(DrawContext& context) override;

    private:
      GraphicsPipeline& _pipeline;
      VkViewport _viewport;
      VkRect2D _scissor;
      uint32_t _pointsNumber;
      Ref<PlainBuffer> _indicesBuffer;
      VolatileUniform<DrawMatrices>& _matricesUniform;
      DrawMatrices _drawMatrices;
      MaxDistanceUniform& _maxDistanceUniform;
      float _maxDistance;
      VolatileUniform<MppxDistanceFunction>& _mppxFunctionUniform;
      MppxDistanceFunction _mppxFunctionValues;
      VolatileUniform<glm::vec2>& _falloffUniform;
      glm::vec2 _falloffValue;
    };
  }
}