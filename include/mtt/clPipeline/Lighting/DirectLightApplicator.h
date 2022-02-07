#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <mtt/clPipeline/Lighting/ShadowMapProvider.h>
#include <mtt/clPipeline/Lighting/DirectLightData.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/render/SceneGraph/DrawableNode.h>

namespace mtt
{
  class LogicalDevice;

  namespace clPipeline
  {
    class DirectLight;

    class DirectLightApplicator : public DrawableNode
    {
    public:
      DirectLightApplicator(DirectLight& light, LogicalDevice& device);
      DirectLightApplicator(const DirectLightApplicator&) = delete;
      DirectLightApplicator& operator = (const DirectLightApplicator&) = delete;
      virtual ~DirectLightApplicator() = default;

      void resetPipelines() noexcept;
      void updateBound() noexcept;

    protected:
      virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      class DrawTechnique
      {
      public:
        DrawTechnique(bool fullscreenRender,
                      DirectLight& light,
                      DirectLightApplicator& applicator);
        DrawTechnique(const DrawTechnique&) = delete;
        DrawTechnique& operator = (const DrawTechnique&) = delete;
        virtual ~DrawTechnique() = default;

        void invalidatePipeline() noexcept;
        void addToDrawPlan( DrawPlanBuildInfo& buildInfo,
                            const ShadowMapProvider::CascadeInfo& cascadeInfo);

      private:
        void _rebuildPipeline(AbstractRenderPass& renderPass);
        void _adjustPipeline();

        void _makeNonshadowCommand( DrawPlanBuildInfo& buildInfo,
                                    uint32_t pointsNumber,
                                    const DirectLightDrawData& lightData);

        void _makeShadowCommand(
                            DrawPlanBuildInfo& buildInfo,
                            uint32_t pointsNumber,
                            const DirectLightDrawData& lightData,
                            const ShadowMapProvider::CascadeInfo& cascadeInfo);

      private:
        bool _fullscreenRender;
        std::optional<GraphicsPipeline> _pipeline;
        DirectLight& _light;
        DirectLightApplicator& _applicator;
      };

    private:
      void _rebuildShadowmapSampler();
      bool _fullscreen(const DrawPlanBuildInfo& buildInfo) const noexcept;

    private:
      LogicalDevice& _device;
      DirectLight& _light;

      Sampler _depthMapSampler;
      Texture2D* _depthTexture;

      Sampler _normalMapSampler;
      Texture2D* _normalTexture;

      Sampler _albedoMapSampler;
      Texture2D* _albedoTexture;

      Sampler _specularMapSampler;
      Texture2D* _specularTexture;

      std::optional<Sampler> _shadowmapSampler;
      std::vector<std::shared_ptr<Texture2D>> _shadowmapTextures;

      VolatileUniform<DirectLightDrawData> _lightDataUniform;

      using CoordsCorrectionData = std::vector<glm::vec4>;
      VolatileUniform<CoordsCorrectionData> _coordsCorrectionUniform;
      VolatileUniform<DrawMatrices> _matricesUniform;

      struct Techniques
      {
        std::unique_ptr<DrawTechnique> shapeTechnique;
        std::unique_ptr<DrawTechnique> fullscreenTechnique;
      };
      Techniques _techniques;
    };
  }
}